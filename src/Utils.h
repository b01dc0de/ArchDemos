#ifndef ARCHDEMOS_UTILS_H
#define ARCHDEMOS_UTILS_H

// Utils.h included in Common.h

namespace Arch
{
#define ARRAY_SIZE(A) (sizeof(A) / sizeof(A[0]))
    using std::string;

    const char* GetBaseDirectory();
    const char* ReadFileContents(const char* Filename);

    void LogMsg(const char* Msg)
    {
        OutputDebugString(Msg);
    }

    void Logf(const char* Fmt, ...)
    {
        static const int MaxMsgSize = 1024;
        static char MsgBuffer[MaxMsgSize];

        va_list args;
        va_start(args, Fmt);
        snprintf(MsgBuffer, MaxMsgSize, Fmt, args);
        va_end(args);

        LogMsg(MsgBuffer);
    }

    void Assert(bool bCondition, const char* Expr, const char* Filename, int LineNo)
    {
        if (!bCondition)
        {
            Logf("ASSERT FAILED!\tin file '%s'\tLine #: %d\n\tExpr: %s\n", Expr);
            DebugBreak();
        }
    }
#define ARCH_ASSERT(Exp) ::Arch::Assert((Exp), #Exp, __FILE__, __LINE__)

    using SizeT = int;
    using IncFactorT = float;
    static const SizeT Array_DefaultInitCapacity = 32;
    static const IncFactorT Array_DefaultIncFactor = 2.0f;
    template <typename ItemT>
    struct Array
    {
        SizeT Capacity = 0;
        SizeT NumItems = 0;
        IncFactorT IncFactor = 0.0f;
        ItemT* Items = nullptr;

        Array(SizeT InitCapacity = Array_DefaultInitCapacity, float InitIncFactor = Array_DefaultIncFactor)
            : Capacity(InitCapacity)
            , NumItems(0)
            , IncFactor(InitIncFactor)
            , Items(new ItemT[InitCapacity]) { }
        Array& operator=(const Array& Other) = delete;
        Array(const Array& Other) = delete;
        Array& operator=(Array&& Other)
        {
            ARCH_ASSERT(nullptr != Other.Items);
            if (Items) { delete[] Items; }

            this->Capacity = Other.Capacity;
            this->NumItems = Other.NumItems;
            this->IncFactor = Other.IncFactor;
            this->Items = Other.Items;

            Other.Capacity = 0;
            Other.NumItems = 0;
            Other.Items = nullptr;

            return *this;
        }
        Array(Array&& Other)
        {
            (void)operator=(Other);
        }
        ~Array() { if (Items) { delete[] Items; } }

        ItemT& operator[](SizeT Idx)
        {
            ARCH_ASSERT(Idx >= 0 && Idx <= Capacity);
            return Items[Idx];
        }

        const ItemT& operator[](SizeT Idx) const
        {
            ARCH_ASSERT(Idx >= 0 && Idx <= Capacity);
            return Items[Idx];
        }

        void Resize(SizeT NewCapacity)
        {
            ARCH_ASSERT(Capacity < NewCapacity);
            if (NewCapacity > Capacity)
            {
                ItemT* NewItems = new ItemT[NewCapacity];
                for (int ItemIdx = 0; ItemIdx < NumItems; ItemIdx++)
                {
                    NewItems[ItemIdx] = Items[ItemIdx];
                }

                delete[] Items;
                Capacity = NewCapacity;
                Items = NewItems;
            }
        }

        void Resize()
        {
            SizeT NewSize = (SizeT)(Capacity * IncFactor);
            Resize(NewSize);
        }

        void Add(ItemT& NewItem)
        {
            if (NumItems == Capacity)
            {
                Resize();
            }
            Items[NumItems++] = NewItem;
        }
    };
}

#endif // ARCHDEMOS_UTILS_H