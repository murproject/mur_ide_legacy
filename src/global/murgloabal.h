#ifndef MURGLOABAL_H
#define MURGLOABAL_H

#include <memory>
#include <mutex>

class MURGlobalPrivate;
class Dispatcher;

class MURGloabal
{
friend class MURGlobalPrivate;

public:
    ~MURGloabal();
    static MURGloabal &instance();

    Dispatcher* dispatcher();

private:
    MURGloabal();
    MURGloabal& operator = (const MURGloabal& rs);
    MURGloabal(const MURGloabal& rs);

    static std::shared_ptr<MURGloabal> m_instance;
    static std::once_flag m_onece;
    static std::shared_ptr<MURGlobalPrivate> m_p;
};

#define GLOBAL MURGloabal::instance()
#define DISPATCHER MURGloabal::instance().dispatcher()

#endif // MURGLOABAL_H
