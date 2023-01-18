#pragma once

#include <memory>
#include <optional>
#include <span>
#include <string>

#include "DescriptorStream.h"

// Credit: https://stackoverflow.com/a/26890188/227663

class Pipe {
	private:
		int descriptors[2];

	public:
		Pipe();
		~Pipe();

		void close();
		inline int readDescriptor()  const { return descriptors[0]; }
		inline int writeDescriptor() const { return descriptors[1]; }
};

int execvpe_hack(const char *program, char **argv, char **env);

class Process {
	private:
		pid_t childPID = -1;
		Pipe readPipe;
		Pipe writePipe;
		Pipe errorPipe;
		std::optional<ifdbuf> readBuffer;
		std::optional<ofdbuf> writeBuffer;
		std::optional<ofdbuf> errorBuffer;

		void init(const char **argv, bool with_path, const char **environment);

	public:
		std::ostream in;
		std::ostream out;
		std::ostream err;

		Process(const char **argv, bool with_path = false, const char **environment = nullptr);
		Process(std::span<std::string_view> argv, bool with_path = false, const char **environment = nullptr);
		Process(std::initializer_list<std::string_view> argv, bool with_path = false,
		        const char **environment = nullptr);

		void sendEOF();
		int wait();
		void writeAll(std::string_view);
		std::string readAll(bool use_stderr = false);
};
