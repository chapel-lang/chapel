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
	2*size*size*size/(3000000*x);
}

parse <- function (size, ampl)
{
	filename = paste("timing-perturbate/pertubate", size, ampl, sep=".");
	
	if (file.exists(filename))
	{
		ret <- scan(filename);
		return(ret);
	};

	return(NULL);
}

parse_ref <- function (size)
{
	filename = paste("timings-sched/sched.greedy", size, sep=".");
	
	if (file.exists(filename))
	{
		ret <- scan(filename);
		return(ret);
	};

	return(NULL);
}

handle_size <- function (size, ampl)
{
	gflops <- gflops(parse(size, ampl), size);

	return(gflops);
}

handle_size_ref <- function (size)
{
	gflops <- gflops(parse_ref(size), size);

	return(gflops);
}



handle_ampl <- function(ampl)
{
	gflopstab <- NULL;
	sizetab <- NULL;

	for (size in sizelist)
	{
		list <- handle_size(size, ampl);
		gflopstab <- c(gflopstab, list);
		sizetab <- c(sizetab, array(size, c(length(list))));
	}

	return(
		data.frame(gflops=gflopstab, size=sizetab, ampl=array(ampl, c(length(gflopstab)) ))
	);
}

handle_ampl_mean <- function(ampl)
{
	meantab <- NULL;
	sizetab <- NULL;

	for (size in sizelist)
	{
		list <- mean(handle_size(size, ampl));
		meantab <- c(meantab, list);
		sizetab <- c(sizetab, array(size, c(length(list))));
	}

	return(
		data.frame(gflops=meantab, size=sizetab, ampl=array(ampl, c(length(meantab)) ))
#		meantab
	);
}

handle_ref_mean <- function()
{
	meantab <- NULL;
	sizetab <- NULL;

	for (size in sizelist)
	{
		list <- mean(handle_size_ref(size));
		meantab <- c(meantab, list);
		sizetab <- c(sizetab, array(size, c(length(list))));
	}

	return(
		data.frame(gflops=meantab, size=sizetab)
#		meantab
	);
}


trace_ampl <- function(ampl, color, style)
{
	#points(handle_ampl(ampl)$size, handle_ampl(ampl)$gflops, col=color);
	lines(handle_ampl_mean(ampl)$size, handle_ampl_mean(ampl)$gflops, type = "o", col=color, lwd= 1, lty=1, pch = style);
}

trace_ref <- function(color)
{
	lines(handle_ref_mean()$size, handle_ref_mean()$gflops, col=color, lwd=3, lty=2);
}

display_ampl <- function()
{
	xlist <- range(sizelist);
	ylist <- range(c(0,100));

	plot.new();
	plot.window(xlist, ylist);

	trace_ref("black");

	trace_ampl("1.0", "black", 4);
	trace_ampl("0.50", "black", 5);
	trace_ampl("0.25", "black", 2);
	trace_ampl("0.1", "black", 1);
	trace_ampl("0.0", "black", 0);



	axis(1, at=sizelist, font=1.6)
	axis(2, at=seq(0, 100, 10), tck=1, font=1.6)
#	axis(4, at=seq(0, 100, 10))
	box(bty="u")

	labels <- c("greedy", "0 %", "10 %", "25 %", "50 %", "100 %")
	legend("topleft", inset=.05, title="Perturbation", labels, lwd=c(2, 1, 1, 1, 1, 1), pch=c(-1, 0, 1, 2, 5, 4), lty=c(2, 1, 1, 1, 1, 1), col=c("black", "black", "black", "black", "black", "black"), bty="y", bg="white")



	mtext("matrix size", side=1, line=2, cex=1.6)
	mtext("GFlops", side=2, line=2, las=0, cex=1.6)

#	title("Impact of performance prediction innacuracies on LU decomposition");

}

display_ampl()
