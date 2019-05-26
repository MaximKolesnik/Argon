#include <chrono>
#include <thread>

#include <type_traits>

#include "time.hpp"

namespace Argon::Core
{
class Time::TimePrivate final
{
public:
	using ClockType = std::conditional_t<
		std::chrono::high_resolution_clock::is_steady,
		std::chrono::high_resolution_clock,
		std::chrono::steady_clock>;
	using TimeStampType = ClockType::time_point;
	using StepPrecision = std::chrono::milliseconds;

	static constexpr auto FPS_CAP = std::chrono::milliseconds(2);

	float32 getDelta() const
	{
		return getUnscaledDelta() * m_deltaScale;
	}

	float32 getUnscaledDelta() const
	{
		return m_lastDelta.count() / 1000.f;
	}

	void setDeltaScale(float32 val)
	{
		m_deltaScale = val;
	}

	float32 getDeltaScale() const
	{
		return m_deltaScale;
	}

	void endFrame()
	{
		updateLastDelta();

		if (m_lastDelta < FPS_CAP)
		{
			std::this_thread::sleep_for(FPS_CAP - m_lastDelta);
			updateLastDelta();
		}

		m_lastUpdate = ClockType::now();
	}

private:
	void updateLastDelta()
	{
		m_lastDelta = std::chrono::duration_cast<StepPrecision>(
			ClockType::now() - m_lastUpdate);
	}

	TimeStampType m_lastUpdate;
	StepPrecision m_lastDelta;
	float32 m_deltaScale = 1.f;
	AR_ATTR_UNUSED byte _padding[4];
};

Time::Time()
	: m_impl(new TimePrivate())
{
}

Time::~Time() = default;

float32 Time::getDelta() const
{
	return m_impl->getDelta();
}

float32 Time::getUnscaledDelta() const
{
	return m_impl->getUnscaledDelta();
}

void Time::setDeltaScale(float32 val)
{
	m_impl->setDeltaScale(val);
}

float32 Time::getDeltaScale() const
{
	return m_impl->getDeltaScale();
}

void Time::endFrame()
{
	m_impl->endFrame();
}
} // namespace Argon::Core
