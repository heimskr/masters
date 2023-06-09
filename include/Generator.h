#pragma once

// Credit: https://en.cppreference.com/w/cpp/coroutine/coroutine_handle

#include <coroutine>
#include <iostream>
#include <optional>

template<std::movable T>
class Generator {
	public:
		struct promise_type {
			Generator<T> get_return_object() {
				return Generator{Handle::from_promise(*this)};
			}

			static std::suspend_always initial_suspend() noexcept {
				return {};
			}

			static std::suspend_always final_suspend() noexcept {
				return {};
			}

			std::suspend_always yield_value(T value) noexcept {
				current_value = std::move(value);
				return {};
			}

			// Disallow co_await in generator coroutines.
			void await_transform() = delete;

			[[noreturn]]
			static void unhandled_exception() { throw; }

			void return_void() {}

			std::optional<T> current_value;
		};

		using Handle = std::coroutine_handle<promise_type>;

		explicit Generator(const Handle coroutine_): coroutine{coroutine_} {}

		Generator() = default;

		~Generator() {
			if (coroutine)
				coroutine.destroy();
		}

		Generator(const Generator &) = delete;
		Generator & operator=(const Generator &) = delete;

		Generator(Generator &&other) noexcept: coroutine{other.coroutine} {
			other.coroutine = {};
		}

		Generator & operator=(Generator &&other) noexcept {
			if (this != &other) {
				if (coroutine)
					coroutine.destroy();
				coroutine = other.coroutine;
				other.coroutine = {};
			}

			return *this;
		}

		// Range-based for loop support.
		class Iter {
			public:
				void operator++() {
					coroutine.resume();
				}

				const T & operator*() const {
					return *coroutine.promise().current_value;
				}

				bool operator==(std::default_sentinel_t) const {
					return !coroutine || coroutine.done();
				}

				explicit Iter(const Handle coroutine): coroutine{coroutine} {}

			private:
				Handle coroutine;
		};

		Iter begin() {
			if (coroutine)
				coroutine.resume();
			return Iter{coroutine};
		}

		std::default_sentinel_t end() {
			return {};
		}

	private:
		Handle coroutine;
};
