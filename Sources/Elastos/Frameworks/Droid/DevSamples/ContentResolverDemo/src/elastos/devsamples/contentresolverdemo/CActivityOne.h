
#ifndef __Elastos_DevSamples_ContentResolverDemo_CActivityOne_H__
#define __Elastos_DevSamples_ContentResolverDemo_CActivityOne_H__

#include <elastos/droid/app/Activity.h>
#include <elastos/droid/database/ContentObserver.h>
#include <Elastos.Droid.Widget.h>
#include "_Elastos_DevSamples_ContentResolverDemo_CActivityOne.h"

using Elastos::Droid::App::Activity;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::Widget::IAdapter;
using Elastos::Droid::Database::ContentObserver;

namespace Elastos {
namespace DevSamples {
namespace ContentResolverDemo {

CarClass(CActivityOne)
    , public Activity
{
public:
    class MyContentObserver
        : public ContentObserver
    {
    public:
        TO_STRING_IMPL("CActivityOne::MyContentObserver")

        MyContentObserver();

        CARAPI constructor(
            /* [in] */ CActivityOne* host);

        //@Override
        CARAPI OnChange(
            /* [in] */ Boolean selfChange);

    private:
        CActivityOne* mHost;
    };

    class MyListener
        : public Object
        , public IViewOnClickListener
    {
    public:
        CAR_INTERFACE_DECL()

        MyListener(
            /* [in] */ CActivityOne* host);

        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        CActivityOne* mHost;
    };

public:
    CAR_OBJECT_DECL()

    CActivityOne();

    ~CActivityOne();

    CARAPI constructor();

protected:

    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI OnStart();

    CARAPI OnResume();

    CARAPI OnPause();

    CARAPI OnStop();

    CARAPI OnDestroy();

private:
    CARAPI OnActivityResult(
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent *data);

    CARAPI InitAdapter();

    CARAPI Add();

    CARAPI Query();

    CARAPI Update();

    CARAPI Delete();

    CARAPI_(Int64) QueryRowId();

    CARAPI UpdateByRowId(
    /* [in] */ Int64 rowId);

    CARAPI UpdateByName(
    /* [in] */ const String& userName);

    CARAPI DeleteByName(
        /* [in] */ const String& name);

private:
    AutoPtr<IAdapter> mAdapter;
    AutoPtr<ICursor> mCursor;
    AutoPtr<MyContentObserver> mContentObserver;
};

} // namespace ContentResolverDemo
} // namespace DevSamples
} // namespace Elastos

#endif // __Elastos_DevSamples_ContentResolverDemo_CActivityOne_H__
