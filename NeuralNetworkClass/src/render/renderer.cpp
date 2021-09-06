GLOBAL_VARIABLE float render_scale = 0.01f;

INTERNAL void clear_screen(u32 color) {
	u32* pixel = (u32*)render_state.memory;
	for (s32 y = 0; y < render_state.height; y++)
	{
		for (s32 x = 0; x < render_state.width; x++)
		{
			*pixel++ = color;
		}
	}
}

INTERNAL void draw_rect_in_pixels(s32 x0, s32 y0, s32 x1, s32 y1, u32 color) {
	
	x0 = clamp(0, x0, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	x1 = clamp(0, x1, render_state.width);
	y1 = clamp(0, y1, render_state.height);
	
	for (s32 y = y0; y < y1; y++)
	{
		u32* pixel = (u32*)render_state.memory + x0 + y*(render_state.width);
		for (s32 x = x0; x < x1; x++)
		{
			*pixel++ = color;
		}
	}
}

INTERNAL void draw_rect(float x, float y, float half_size_x, float half_size_y, u32 color) {
	// change to pixel

	x *= render_state.width * render_scale;
	y *= render_state.height * render_scale;
	half_size_x *= render_state.width * render_scale;
	half_size_y *= render_state.height * render_scale;

	x += render_state.width / 2.0f;
	y += render_state.height / 2.0f;

	s32 x0 = x - half_size_x;
	s32 x1 = x + half_size_x;
	s32 y0 = y - half_size_y;
	s32 y1 = y + half_size_y;

	draw_rect_in_pixels(x0, y0, x1, y1, color);
}