#include "MiniRt.h"
#include "garbage_collector/my_malloc.h"

t_scene	*linking_fucntion(t_scene *scene, bool set)
{
	static t_scene	*local;

	if (set == true)
	{
		local = scene;
		return (scene);
	}
	return (local);
}

void	init_input_data(t_objects_fb *input_data)
{
	input_data->assign_object = NULL;
	input_data->identifier = NULL;
	input_data->nb = 0;
	input_data->next = NULL;
}

void	set_scene(t_scene *scene, bool flag)
{
	static t_cleanup	*cleanup = NULL;

	if (!flag)
		cleanup = scene->cleaner;
	else if (cleanup)
		cleanup->flag_input = true;
}

void	init_cleaner(t_cleanup *cleaner)
{
	cleaner->container = NULL;
	cleaner->dispatched_table = NULL;
	cleaner->scene = NULL;
	cleaner->token_count = 0;
	cleaner->tokens = NULL;
	cleaner->flag_input = false;
	cleaner->flag_exit = false;
	cleaner->input_data = NULL;
}

void	parsing_second_part(char ***tokens, int *counter, \
		t_cleanup *cleaner)
{
	t_scene			*scene;
	t_scene			*holder;
	t_objects_fb	*input_data;
	t_objects_fb	*dispatch_table;

	dispatch_table = NULL;
	scene = alloc(sizeof (t_scene), false);
	input_data = alloc(sizeof (t_objects_fb), false);
	init_input_data(input_data);
	cleaner->input_data = input_data;
	init_object_dispatch_table(&dispatch_table);
	cleaner->dispatched_table = &dispatch_table;
	scene->cleaner = cleaner;
	cleaner->scene = scene;
	initialize_scenes(scene);
	start_asigning_objects(scene, dispatch_table, tokens,*counter);
	make_sure_of_objects(scene, scene->cleaner);
	set_scene(scene, false);
	holder = linking_fucntion(scene, true);
	jassim_mlx(holder);
}
