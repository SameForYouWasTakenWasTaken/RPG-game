#pragma once

inline unsigned int GetID(const char* /*unused*/) {
    static unsigned int count = 0;
    return count++;
}

#define EVENT_TYPE(X) \
static unsigned int GetStaticType(){ \
    static unsigned int id = GetID(#X); \
    return id; \
}


namespace Core::Events
{
    class IEvent
    {
    public:
        virtual bool operator==(const IEvent& other)
        {
            if (GetStaticType() == other.GetStaticType())
                return true;
            return false;
        }

        EVENT_TYPE(IEvent);
    };
}