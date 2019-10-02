
mpi <- read.table("output_mpi.dat", sep = "", na.strings ="", stringsAsFactors= F)
names(mpi) = c("Processors", "Time")
mpi["Speedup"] <- mpi$Time[1]/mpi$Time

omp <- read.table("output_omp.dat", sep = "", na.strings ="", stringsAsFactors= F)
names(omp) = c("Processors", "Time")
omp["Speedup"] <- omp$Time[1]/omp$Time

write.table(mpi, "processed_mpi.dat", sep="\t", row.names = F, col.names = F) 
write.table(omp, "processed_omp.dat", sep="\t", row.names = F, col.names = F) 
