struct starpu_codelet cl = {
	.where = STARPU_CPU,
	/* => .modes = { STARPU_R, STARPU_W }, */
	.modes[1] = STARPU_W,
	.modes[0] = STARPU_R,
	.cpu_func = foo
};


static void
foo(void)
{
	struct starpu_codelet cl = {
		.where = STARPU_CPU,
		/* .modes = STARPU_R, STARPU_RW, STARPU_W } */
		.modes[2] = STARPU_W,
		.modes[1] = STARPU_RW,
		.modes[0] = STARPU_R
	};
}
