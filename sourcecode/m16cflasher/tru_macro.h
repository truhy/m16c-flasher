#ifndef TRU_MACRO_H
#define TRU_MACRO_H

#define ENUM_ITEM(name, value) \
	name,

#define ENUM_ITEM_VALUE(name, value) \
	value,

#define CREATE_ENUM(name, list) \
	enum name { \
		list(ENUM_ITEM) \
	};

#define INIT_CLASS_ARRAY_ENUM(class_name, array_type, array_name, list) \
	array_type class_name::array_name[] = { \
		list(ENUM_ITEM_VALUE) \
	};

#define INIT_INLINE_CLASS_ARRAY_ENUM(array_type, array_name, list) \
	array_type array_name[] = { \
		list(ENUM_ITEM_VALUE) \
	};

#endif
