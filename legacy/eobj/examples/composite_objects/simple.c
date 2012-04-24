#include "Eobj.h"
#include "simple.h"

#include "config.h"

EAPI Eobj_Op SIMPLE_BASE_ID = 0;

EAPI const Eobj_Event_Description _SIG_A_CHANGED =
        EOBJ_EVENT_DESCRIPTION("a,changed", "i", "Called when a has changed.");

#define MY_CLASS SIMPLE_CLASS

static void
_a_set(Eobj *obj, void *class_data, va_list *list)
{
   Simple_Public_Data *pd = class_data;
   int a;
   a = va_arg(*list, int);
   printf("%s %d\n", eobj_class_name_get(MY_CLASS), a);
   pd->a = a;

   eobj_event_callback_call(obj, SIG_A_CHANGED, &pd->a);
}

static void
_a_get(const Eobj *obj EINA_UNUSED, const void *class_data, va_list *list)
{
   const Simple_Public_Data *pd = class_data;
   int *a;
   a = va_arg(*list, int *);
   *a = pd->a;
}

static void
_class_constructor(Eobj_Class *klass)
{
   const Eobj_Op_Func_Description func_desc[] = {
        EOBJ_OP_FUNC(SIMPLE_ID(SIMPLE_SUB_ID_A_SET), _a_set),
        EOBJ_OP_FUNC_CONST(SIMPLE_ID(SIMPLE_SUB_ID_A_GET), _a_get),
        EOBJ_OP_FUNC_SENTINEL
   };

   eobj_class_funcs_set(klass, func_desc);
}

static const Eobj_Op_Description op_desc[] = {
     EOBJ_OP_DESCRIPTION(SIMPLE_SUB_ID_A_SET, "i", "Set property A"),
     EOBJ_OP_DESCRIPTION_CONST(SIMPLE_SUB_ID_A_GET, "i", "Get property A"),
     EOBJ_OP_DESCRIPTION_SENTINEL
};

static const Eobj_Event_Description *event_desc[] = {
     SIG_A_CHANGED,
     NULL
};

static const Eobj_Class_Description class_desc = {
     "Simple",
     EOBJ_CLASS_TYPE_REGULAR,
     EOBJ_CLASS_DESCRIPTION_OPS(&SIMPLE_BASE_ID, op_desc, SIMPLE_SUB_ID_LAST),
     event_desc,
     sizeof(Simple_Public_Data),
     NULL,
     NULL,
     _class_constructor,
     NULL
};

EOBJ_DEFINE_CLASS(simple_class_get, &class_desc, EOBJ_BASE_CLASS, NULL);

