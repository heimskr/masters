#include <sstream>
#include <sys/wait.h>
#include <unistd.h>

#include "Process.h"

// Credit for Pipe and Process: https://stackoverflow.com/a/26890188/227663

Pipe::Pipe() {
	if (pipe(descriptors) != 0)
		throw std::runtime_error("Couldn't open pipe");
}

Pipe::~Pipe() {
	close();
}

void Pipe::close() {
	::close(descriptors[0]);
	::close(descriptors[1]);
}

extern char **environ;

// Credit for execvpe_hack: https://stackoverflow.com/a/7789795/227663
int execvpe_hack(const char *program, char **argv, char **envp) {
	char **saved = environ;
	int result;
	environ = envp;
	result = execvp(program, argv);
	environ = saved;
	return result;
}

Process::Process(const char **argv, bool with_path, const char **environment):
in(nullptr), out(nullptr), err(nullptr) {
	init(argv, with_path, environment);
}

Process::Process(std::span<std::string_view> argv, bool with_path, const char **environment):
in(nullptr), out(nullptr), err(nullptr) {
	std::unique_ptr<const char *[]> unique_argv = std::make_unique<const char *[]>(argv.size() + 1);

	for (size_t i = 0; i < argv.size(); ++i)
		unique_argv[i] = argv[i].begin();
	unique_argv[argv.size()] = nullptr;

	init(unique_argv.get(), with_path, environment);
}

Process::Process(std::initializer_list<std::string_view> argv, bool with_path, const char **environment):
in(nullptr), out(nullptr), err(nullptr) {
	std::unique_ptr<const char *[]> unique_argv = std::make_unique<const char *[]>(argv.size() + 1);

	for (size_t i = 0; i < argv.size(); ++i)
		unique_argv[i] = (argv.begin() + i)->begin();
	unique_argv[argv.size()] = nullptr;

	init(unique_argv.get(), with_path, environment);
}

void Process::init(const char **argv, bool with_path, const char **environment) {
	childPID = fork();

	if (childPID == -1)
		throw std::runtime_error("Failed to start child process");

	if (childPID == 0) {
		dup2(writePipe.readDescriptor(), STDIN_FILENO);
		dup2(readPipe.writeDescriptor(), STDOUT_FILENO);

		writePipe.close();
		readPipe.close();

		int result;

		if (with_path) {
			if (environment != nullptr)
				result = execvpe_hack(argv[0], const_cast<char **>(argv), const_cast<char **>(environment));
			else
				result = execvp(argv[0], const_cast<char **>(argv));
		} else if (environment != nullptr)
			result = execve(argv[0], const_cast<char **>(argv), const_cast<char **>(environment));
		else
			result = execv(argv[0], const_cast<char **>(argv));

		if (result == -1)
			throw std::runtime_error("Failed to launch program");
	} else {
		::close(writePipe.readDescriptor());
		::close(readPipe.writeDescriptor());
		readBuffer.emplace(readPipe.readDescriptor());
		writeBuffer.emplace(writePipe.writeDescriptor());
		errorBuffer.emplace(errorPipe.writeDescriptor());
		in.rdbuf(&*writeBuffer);
		out.rdbuf(&*readBuffer);
		err.rdbuf(&*errorBuffer); // TODO!: verify
	}
}

void Process::sendEOF() {
	writeBuffer->close();
}

int Process::wait() {
	int status;
	waitpid(childPID, &status, 0);
	return status;
}

void Process::writeAll(std::string_view input) {
	in << input;
	sendEOF();
}

std::string Process::readAll(bool use_stderr) {
	std::stringstream ss;
	ss << (use_stderr? err : out).rdbuf();
	return ss.str();
}