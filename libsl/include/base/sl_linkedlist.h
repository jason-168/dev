/*
 * sl_linkedlist.h
 *
 *  Created on: May 30, 2016
 *      Author: root
 */

#ifndef SL_LINKEDLIST_H_
#define SL_LINKEDLIST_H_

#include <stdbool.h>
#include <stdint.h>

#include <base/sl_base.h>
#include <base/sl_list.h>

#include <base/sl.h>


SL_BEGIN_DECLS

typedef void ele_autorelease_proc(sl_base **e);

typedef struct sl_linkedlist{
	struct list_head _linkedhead; //list head

	ele_autorelease_proc *_release_proc;

	int32_t _size;
}sl_linkedlist;

typedef struct sl_linkedele{
	struct list_head _head; //list entry
	sl_base *_object;
}sl_linkedele;

sl_status_t sl_linkedlist_init(sl_linkedlist *list, ele_autorelease_proc *proc);

sl_status_t sl_linkedlist_add(sl_linkedlist *list, sl_base *e);

sl_base* sl_linkedlist_removeFirst(sl_linkedlist *list);
sl_base* sl_linkedlist_remove(sl_linkedlist *list,
		bool (*equals)(sl_base *e, void* context), void* context);

sl_status_t sl_linkedlist_del(sl_linkedlist *list, sl_base *e);
sl_status_t sl_linkedlist_del2(sl_linkedlist *list, struct list_head *pos);

sl_status_t sl_linkedlist_del3(sl_linkedlist *list,
		bool (*equals)(sl_base *e, void* context), void* context);

bool sl_linkedlist_contains(sl_linkedlist *list, sl_base *e);

void sl_linkedlist_clear(sl_linkedlist *list);

int32_t sl_linkedlist_size(sl_linkedlist *list);

sl_base* sl_linkedlist_getFirst(sl_linkedlist *list);
sl_base* sl_linkedlist_getLast(sl_linkedlist *list);

sl_base* sl_linkedlist_get(sl_linkedlist *list,
		bool (*equals)(sl_base *e, void* context), void* context);

sl_base* sl_linkedlist_getByIndex(sl_linkedlist *list, int index);

void sl_linkedlist_forEach(sl_linkedlist *list,
        bool (*callback)(struct list_head *pos, sl_base *e, void* context),
        void* context);

#define sl_linkedlist_for(pos, list)		\
	struct list_head* _##pos = NULL;				\
	struct list_head* _##pos##tmp = NULL;			\
	list_for_each_safe(_##pos, _##pos##tmp, &(list)->_linkedhead)

//not safe
#define sl_linkedlist_for_prev(pos, list)		\
		struct list_head* _##pos = NULL;		\
		list_for_each_prev(_##pos, &(list)->_linkedhead)

#define sl_linkedlist_entry(pos)					\
	(list_entry(_##pos, sl_linkedele, _head)->_object)

#define sl_linkedlist_del_pos(list, pos)	\
		sl_linkedlist_del2(list, _##pos)


//---------------------------------------------------------------------
bool linkedlist_item_equals(sl_base *e, void* context);


SL_END_DECLS

#endif /* SL_LINKEDLIST_H_ */
