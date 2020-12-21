#include "murgloabal.h"
#include "dispatcher.h"

std::once_flag MURGloabal::m_onece;
std::shared_ptr<MURGloabal> MURGloabal::m_instance = nullptr;
std::shared_ptr<MURGlobalPrivate> MURGloabal::m_p = nullptr;

class MURGlobalPrivate
{
public:
    ~MURGlobalPrivate();
private:
    friend class MURGloabal;

    MURGlobalPrivate();

    Dispatcher* m_dispatcher;
};

MURGlobalPrivate::MURGlobalPrivate()
    : m_dispatcher(new Dispatcher)
{

}

MURGlobalPrivate::~MURGlobalPrivate()
{
    delete m_dispatcher;
}


MURGloabal &MURGloabal::instance()
{
    std::call_once( MURGloabal::m_onece,
                    []() {
                        MURGloabal::m_instance.reset(new MURGloabal());
                    });
    return *MURGloabal::m_instance;
}

Dispatcher *MURGloabal::dispatcher()
{
    return m_p->m_dispatcher;
}

MURGloabal::MURGloabal()
{
    m_p.reset(new MURGlobalPrivate);
}

MURGloabal &MURGloabal::operator =(const MURGloabal &rs)
{
    if (this != &rs) {
        m_instance = rs.m_instance;
    }
    return *this;
}

MURGloabal::MURGloabal(const MURGloabal &rs)
{
    m_instance = rs.m_instance;
}

MURGloabal::~MURGloabal()
{

}
