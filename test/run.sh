DDAT_DIR=/home/jinyuyang/PACMAN_PROJECT/huawei21/data-dependence-analysis-tool

LLVM_VERSION=                                                                                                                                                
                                                                                                                                                                  
CLANG=clang$LLVM_VERSION
CLANGXX=clang++$LLVM_VERSION                                                                                                                                          
LLVM_DIS=llvm-dis$LLVM_VERSION                                                                                                                                    
OPT=opt$LLVM_VERSION                                                                                                                                              
LLC=llc$LLVM_VERSION                                                                                                                                              
#CC=mpicxx                                                                                                                                                         

PREFIX=$1
                                                                                                                                                          
$CLANGXX -g -c -emit-llvm -O1 -o $PREFIX.bc $DDAT_DIR/test/$PREFIX.cpp                                                                                                  
$LLVM_DIS $PREFIX.bc                                                                                                                                                 
                                                                                                                                                                  
$OPT -loop-simplify -mergereturn $PREFIX.bc -o $PREFIX.opt.bc                                                                                                         
$LLVM_DIS $PREFIX.opt.bc                                                                                                                                           
                                                                                                                                                                  
$OPT -load $DDAT_DIR/build/src/DDAPass.so -DDAPass $PREFIX.opt.bc -enable-new-pm=0 -o $PREFIX.final.bc                                                                            
$LLVM_DIS $PREFIX.final.bc   

# $LLC -filetype=obj $PREFIX.final.bc -o $PREFIX.final.o

# $CLANGXX -O3 $PREFIX.final.o -o $PREFIX.final -lmpi