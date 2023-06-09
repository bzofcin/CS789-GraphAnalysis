/*!
 * ProcessJTest::MulticoreTest declaration
 *
 * \author Alexander C. Thomason
 * \author Carlos L. Cuenca
 * \date 03/13/2022
 * \version 1.0.0
 */

#ifndef UNLV_PROCESS_J_MULTICORE_TEST_HPP
#define UNLV_PROCESS_J_MULTICORE_TEST_HPP

/* TODO:
 * ---
 * this test doesn't quite work, as we need to isolate the threads earlier
 * in the multicore scheduler. see pj_process_tests.cpp for more details
 */

namespace ProcessJTest {

    class mc_process;
    class multicore_test;

}

class ProcessJTest::mc_process : public ProcessJRuntime::pj_process
    {
    public:
        mc_process() = delete;
        mc_process(int32_t id, ProcessJRuntime::pj_scheduler* sched)
        : id(id)
        {
            this->sched = sched;
        }
        virtual ~mc_process() = default;

        void run()
        {
            switch(get_label())
            {
                case 0: goto L0;   break;
                case 1: goto LEND; break;
            }
        L0:
            {
                std::cout << "Hello from L0! (proc " << id << " on cpu "
                          << sched_getcpu() << ")\n";
                /* this is the code that should be generated by a timeout
                 */
                // std::chrono::system_clock::time_point tp =
                //     std::chrono::system_clock::now() + std::chrono::seconds(3);
                ProcessJRuntime::pj_timer* timer = new ProcessJRuntime::pj_timer(this, 3);
                this->sched->insert(timer);
                this->set_not_ready();
                set_label(1);
                return;
            }
        LEND:
            {
                std::cout << "END (proc " << id << " on cpu "
                          << sched_getcpu() << ")\n";
                terminate();
                return;
            }
        }

        friend std::ostream& operator<<(std::ostream& o, mc_process& p)
        {
            o << p.id;
            return o;
        }
    private:
        int32_t id;
        ProcessJRuntime::pj_scheduler* sched;
    };

    class ProcessJTest::multicore_test
    {
    public:
        multicore_test()
        {
            std::cout << "instantiating test...\n";
        }

        void run()
        {
            std::cout << "\n *** CREATING SCHEDULER *** \n\n";
            std::vector<ProcessJRuntime::pj_scheduler> schedulers(4);

            std::cout << "\n *** CREATING PROCESSES *** \n\n";
            ProcessJTest::mc_process* processes[4];
            int32_t i;
            for(i = 0; i < 4; ++i)
            {
                processes[i] = new ProcessJTest::mc_process(i, &schedulers[i]);
            }

            std::cout << "\n *** SCHEDULING PROCESS *** \n\n";
            for(i = 0; i < 4; ++i)
            {
                schedulers[i].insert(processes[i]);
            }

            std::cout << "\n *** STARTING SCHEDULER *** \n\n";
            for(i = 0; i < 4; ++i)
            {
                schedulers[i].start();
            }
        }
    };


#endif
