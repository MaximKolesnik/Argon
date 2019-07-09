#include <chrono>
#include <thread>

#include <type_traits>

#include "time.hpp"

namespace Argon
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

	float32 GetDelta() const
	{
		return GetUnscaledDelta() * m_deltaScale;
	}

	float32 GetUnscaledDelta() const
	{
		return m_lastDelta.count() / 1000.f;
	}

	void SetDeltaScale(float32 val)
	{
		m_deltaScale = val;
	}

	float32 GetDeltaScale() const
	{
		return m_deltaScale;
	}

	void EndFrame()
	{
		UpdateLastDelta();

		if (m_lastDelta < FPS_CAP)
		{
			std::this_thread::sleep_for(FPS_CAP - m_lastDelta);
			UpdateLastDelta();
		}

		m_lastUpdate = ClockType::now();
	}

private:
	void UpdateLastDelta()
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

float32 Time::GetDelta() const
{
	return m_impl->GetDelta();
}

float32 Time::GetUnscaledDelta() const
{
	return m_impl->GetUnscaledDelta();
}

void Time::SetDeltaScale(float32 val)
{
	m_impl->SetDeltaScale(val);
}

float32 Time::GetDeltaScale() const
{
	return m_impl->GetDeltaScale();
}

void Time::EndFrame()
{
	m_impl->EndFrame();
}
} // namespace Argon
