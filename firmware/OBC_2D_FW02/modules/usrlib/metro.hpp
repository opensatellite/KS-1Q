#pragma once

class metro
{
private:
	uint32_t _last;
	uint32_t _interval;
public:
	metro(uint32_t interval)
	{
		_last = millis();
		_interval = interval;
	}
	bool operater()
	{
		uint32_t t = millis();
		uint32_t dt = t - _last;
		if (dt > _interval)
		{
			_last = t;
			return true;
		}
		else
		{
			return false;
		}
	}
	void reset()
	{
		_last = millis();
	}
	void set_interval(uint32_t interval)
	{
		_interval = interval;
	}
	int eta()
	{
		uint32_t dt = millis() - _last;
		return (dt > _interval) ? 0 : dt;
	}
	int time_wait()
	{
		return (millis() - _last);
	}
};
