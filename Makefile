APP=airnav_ais

CFG_FILE=airnav_ais.cfg

all: $(APP)

.PHONY:all clean install

CXX = arm-linux-gcc

OBJS= net_tcp_srv.o $(APP).o serport.o xml_cfg.o

EXT_LIB=libtinyxml.a


arm_linux_lib=/opt/FriendlyARM/toolschain/4.5.1/arm-none-linux-gnueabi
arm_nfs_root =/home/ybb/arm_6410/nfsroot/tiny6410


boost_include_dir =/home/ybb/arm_6410/libs/boost_1_53_0
boost_lib_dir=/home/ybb/arm_6410/libs/boost_1_53_0/stage/lib

%.o:%.cc
	arm-linux-gcc -I$(boost_include_dir)  -I./  -c $<

$(APP):$(OBJS)
	$(CXX)  -I./ -L$(arm_linux_lib) -L. -L$(boost_lib_dir)   -lstdc++ -lpthread   -lboost_system -o $@  $(OBJS)  $(EXT_LIB)




clean:
	-rm *.o $(APP)

install:
	-rm $(arm_nfs_root)/$(APP)
	-rm $(arm_nfs_root)/etc/$(CFG_FILE)
	cp ./$(APP) $(arm_nfs_root)
	cp ./$(CFG_FILE)  $(arm_nfs_root)/etc