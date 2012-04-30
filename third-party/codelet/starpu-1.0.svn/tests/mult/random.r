
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

schedlist <- c("greedy", "random");
sizelist <- seq(2048, 16384, 1024);
#sizelist <- seq(2048, 16384, 2048);

print(schedlist);

print(sizelist);

gflops <- function (x, size)
{
	(2*size*size*size)/(1000000*x);
}

parse <- function (size, sched)
{
	filename = paste("timings-sched/sched", sched, size, sep=".");

	if (file.exists(filename))
	{	ret <- scan(paste("timings-sched/sched", sched, size, sep="."));
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

handle_sched_max <- function(sched)
{
	gflopstab <- NULL;
	sizetab <- NULL;

	for (size in sizelist)
	{
		prout <- handle_size(size, sched);
		list <- max(prout);
		print(list);
		gflopstab <- c(gflopstab, list);
		sizetab <- c(sizetab, size);
	}

	return(
		data.frame(gflops=gflopstab, size=sizetab, sched=array(sched, c(length(gflopstab)) ))
	);
}


handle_sched_min <- function(sched)
{
	gflopstab <- NULL;
	sizetab <- NULL;

	for (size in sizelist)
	{
		list <- min((handle_size(size, sched)));
		print("MIN"); print( list);
		gflopstab <- c(gflopstab, list);
		sizetab <- c(sizetab, size);
	}

	return(
		data.frame(gflops=gflopstab, size=sizetab, sched=array(sched, c(length(gflopstab)) ))
	);
}




trace_sched <- function(sched, color, style, prout)
{
	#lines(handle_sched_mean(sched)$size, handle_sched_mean(sched)$gflops, col=color, legend.text=TRUE);
	if (length(handle_sched_mean(sched)))
	{
		if (prout)
		{
			#for (size in sizelist)
			#{
			#	#lines(array(size, c(length(  handle_size(size, sched) )) ), handle_size(size, sched));
			#}

			convexx <- NULL;
			convexy <- NULL;

			for (point in (handle_sched_min(sched)$size))
			{
				convexx <- c(convexx, point);
			}

			for (point in (handle_sched_min(sched)$gflops))
			{
				convexy <- c(convexy, point);
			}

			for (point in (handle_sched_max(sched)$size))
			{
				convexx <- c(point, convexx);
			}

			for (point in (handle_sched_max(sched)$gflops))
			{
				convexy <- c(point, convexy);
			}

			#lines(handle_sched_min(sched)$size, handle_sched_min(sched)$gflops);
			#lines(handle_sched_max(sched)$size, handle_sched_max(sched)$gflops);

			polygon(convexx, convexy, col="light gray", border=-1);

			lines(handle_sched_mean(sched)$size, handle_sched_mean(sched)$gflops, col=color, type = "o", pch=style, lty=2, lwd=2);
		}
		else 
		{
			lines(handle_sched_mean(sched)$size, handle_sched_mean(sched)$gflops, col=color, type = "o", pch=style, lwd=2);
		}
	};
}

display_sched <- function()
{
	xlist <- range(sizelist);
	ylist <- range(c(0,110));

	plot.new();
	plot.window(xlist, ylist);

	trace_sched("random", "blue",1, 1);
	#trace_sched("dm", "black", 0, 0);
	trace_sched("greedy", "red", 2, 0);

	axis(1, at=sizelist)
	axis(2, at=seq(0, 120, 10), tck=1, cex=1.6)
#	axis(4, at=seq(0, 120, 10))
	box(bty="u", cex=1.6)

        labels <- c("greedy", "weighted random (mean)")
	legend("bottomright", inset=.05, title="Scheduling policy", labels, lwd=2, pch=c(2, 1),lty=c(1, 2), col=c("red", "blue" ), bty="y", bg="white", cex=1.6)


	mtext("matrix size", side=1, line=2, cex=1.6)
	mtext("GFlops", side=2, line=2, las=0, cex=1.6)

	title("Matrix Multiplication");

}

display_sched()
