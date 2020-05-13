#pragma once

namespace argon
{
class NonCopyable
{
public:
	NonCopyable(const NonCopyable &) = delete;
	NonCopyable& operator=(const NonCopyable &) = delete;

protected:
	NonCopyable() = default;
	NonCopyable(NonCopyable &&) = default;
	NonCopyable& operator=(NonCopyable &&) = default;
	~NonCopyable() = default;
};
} // namespace argon
