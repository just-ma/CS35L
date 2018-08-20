Justin Ma
Lab 6
	Thinking of a way to split up the work was the most difficult part. I initially 
thought of dividing the picture into blocks, but that would run into complications when 
dividing left remainders. I finally discovered that if there were n threads, and each 
thread worked on every n pixel, with each thread spaced evenly apart at the beginning, 
there would be no need to worry about remainders. 

	As for compiler errors, I ran into issues surrounding incompatible parameter 
types, since pointers can be tricky. For example I first created a void function instead 
of a void* function for threading.

	Looking at the performance, I think my implementation did a great job improving 
the ray tracer's efficiency. When doubling the number of threads, the real time was 
approximately cut in half, which is a very good sign.
