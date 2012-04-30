
# StarPU --- Runtime system for heterogeneous multicore architectures.
#
# Copyright (C) 2010  Université de Bordeaux 1
# Copyright (C) 2010, 2011  Centre National de la Recherche Scientifique
#
# StarPU is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or (at
# your option) any later version.
#
# StarPU is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# See the GNU Lesser General Public License in COPYING.LGPL for more details.

sizelist <- seq(2048, 24576, 2048);
schedlist <- c("greedy", "prio", "dm", "random");

print(schedlist);

print(sizelist);

gflops <- function (x, size)
{
	size*size*size/(3000000*x);
}

parse <- function (size, sched)
{
	filename = paste("timings_sched/sched", sched, size, sep=".");

	if (file.exists(filename))
	{
		ret <- scan(paste("timings_sched/sched", sched, size, sep="."));
		return(ret);
	};

	return(NULL);
}

handle_size <- function (size, sched)
{
	gflops <- gflops(parse(size, sched), size);

	return(gflops);
}


handle_sched <- function(sched)
{
	gflopstab <- NULL;
	sizetab <- NULL;

	for (size in sizelist)
	{
		list <- handle_size(size, sched);
		gflopstab <- c(gflopstab, list);
		sizetab <- c(sizetab, array(size, c(length(list))));
	}

	return(
		data.frame(gflops=gflopstab, size=sizetab, sched=array(sched, c(length(gflopstab)) ))
	);
}

handle_sched_mean <- function(sched)
{
	meantab <- NULL;
	sizetab <- NULL;

	for (size in sizelist)
	{
		list <- mean(handle_size(size, sched));
		meantab <- c(meantab, list);
		sizetab <- c(sizetab, array(size, c(length(list))));
	}

	return(
		data.frame(gflops=meantab, size=sizetab, sched=array(sched, c(length(meantab)) ))
#		meantab
	);
}

trace_sched <- function(sched, color, style)
{
#	points(handle_sched(sched)$size, handle_sched(sched)$gflops, col=color);
	#lines(handle_sched_mean(sched)$size, handle_sched_mean(sched)$gflops, col=color, legend.text=TRUE);
	lines(handle_sched_mean(sched)$size, handle_sched_mean(sched)$gflops, col=color, pch = style, type ="o");
}

display_sched <- function()
{
	xlist <- range(sizelist);
	ylist <- range(c(0,80));

	plot.new();
	plot.window(xlist, ylist);

	trace_sched("dm", "black", 0);
	trace_sched("prio", "black", 4);
#	trace_sched("random", "black", 1);
	trace_sched("no-prio", "black", 5);
	trace_sched("greedy", "red", 2);

	axis(1, at=sizelist)
	axis(2, at=seq(0, 100, 10), tck=1)
#	axis(4, at=seq(0, 100, 10))
	box(bty="u")

 #      labels <- c("greedy", "priority", "model", "random", "no-prio")
#        labels <- c("model", "priority", "weigthed random", "no priority")
        labels <- c("model", "priority", "no priority")
#	col <-  c("red", "blue", "green", "orange", "black")
        legend("topleft", inset=.05, title="Scheduling policy", labels, lwd=1, lty=1, pch=c(0, 4, 5, 2), col="black", bty="y", bg="white")

        mtext("matrix size", side=1, line=2, cex=1.6)
        mtext("GFlops", side=2, line=2, las=0, cex=1.6)



#	title("Impact of the scheduling strategy on Cholesky decomposition");

}

display_sched()
