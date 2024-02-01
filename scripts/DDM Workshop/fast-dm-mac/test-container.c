/* test-container.c - unit tests for container.c
 *
 * Copyright (C) 2013  Jochen Voss, Andreas Voss.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

#undef NDEBUG

#include "container.c"

#include <assert.h>

int
main()
{
	struct set *set;
	char  name[6] = "item \0";
	int  a, b, c;

	set = new_set();
	a = set_item(set, "a", 1);
	b = set_item(set, "b", 1);
	assert(set_item(set, "a", 0) == a);
	assert(set_item(set, "a", 1) == a);
	name[5] = '\0';
	for (name[4]=' '; name[4]<='~'; ++name[4])
		set_item(set, name, 1);
	assert(set_item(set, "b", 0) == b);
	assert(set_item(set, "b", 1) == b);
	assert(set_item(set, "c", 0) == -1);
	c = set_item(set, "c", 1);
	assert(set_item(set, "c", 0) == c);
	assert(set_item(set, "c", 1) == c);
	assert(a >= 0 && b >= 0 && c >= 0);
	delete_set(set);

	struct dict *dict;
	dict = new_dict();
	dict_add(dict, "key_a", "val_a");
	for (name[4]=' '; name[4]<='~'; ++name[4])
		dict_add(dict, name, "val_xxx");
	assert(strcmp(dict_lookup(dict, "key_a"), "val_a") == 0);
	assert(dict_lookup(dict, "key_b") == NULL);
	dict_add(dict, "key_b", "val_b");
	assert(strcmp(dict_lookup(dict, "key_b"), "val_b") == 0);
	dict_clear(dict);
	assert(dict_lookup(dict, "key_b") == NULL);
	delete_dict(dict);

	struct array *array;
	array = new_array();
	array_append(array, "test");
	for (name[4]='~'; name[4]>=' '; --name[4])
		array_append(array, name);
	array_append(array, "aaa");
	assert(array_find(array, "test") == 0);
	assert(array_find(array, "aaa") > 0);
	assert(array_find(array, "zzz") < 0);
	array_sort(array);
	assert(array_find(array, "test") > 0);
	assert(strcmp(array->entry[array_find(array, "test")], "test") == 0);
	assert(array_find(array, "aaa") == 0);
	assert(array_find(array, "zzz") < 0);
	array_clear(array);
	assert(array_find(array, "test") < 0);
	delete_array(array);

	return 0;
}
