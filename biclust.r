#!/usr/bin/env Rscript
library('biclust')

args <- commandArgs(trailingOnly = TRUE)
data.table <- read.table(args)
data=as.matrix(data.table)

cnum=10
res<-biclust(data,method=BCCC(),number=cnum,alpha=1.0,delta=0.1)
#res<-biclust(data,method=BCBimax(),number=20)
options(width=10000)
print(bicluster(data,res))
options(width=80)
print(biclusternumber(res))

for(i in 1:res@Number){
	name=paste("drawHeatmap",i,".png",sep="")
	png(name)
	drawHeatmap(data,res,number=i)
	dev.off()
	print(paste(name,"created"))
}

for(i in 1:res@Number){
	name=paste("heatmapBC",i,".png",sep="")
	png(name)
	heatmapBC(data,res,order=TRUE,number=i)
	dev.off()
	print(paste(name,"created"))
}

filename=paste(args,"_raw.txt",sep="")
write.table(data.table, file=filename, quote=F, row.names=F, col.names=F, sep='\t')
print(paste(filename,"created"))