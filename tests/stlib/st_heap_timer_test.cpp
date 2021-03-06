#include "../include/st_heap_timer.h"

ST_NAMESPACE_USING

TEST(StStatus, notify)
{
	HeapTimer c;

	c.Start(new StTimerEntry(), 100);
	c.Start(new StTimerEntry(), 160);
	c.Start(new StTimerEntry(), 1500);
	c.Start(new StTimerEntry(), 99);
	c.Start(new StTimerEntry(), 20);
	c.Start(new StTimerEntry(), 5000);
	c.Start(new StTimerEntry(), 999);
	c.Start(new StTimerEntry(), 900);
	c.Start(new StTimerEntry(), 10000);

	LOG_TRACE("==========================");
	int count = 10;
	while(count-- > 0)
	{
		c.CheckExpired();

		int64_t now_ms = Util::SysMs();
        LOG_TRACE("now_ms = %llu", now_ms);

		Util::SysSleep(1);
	}
}

// 测试所有的功能
int main(int argc, char* argv[])
{
    return RUN_ALL_TESTS();
}