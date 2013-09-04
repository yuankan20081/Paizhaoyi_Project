PZY_SVR: main.o Log.o Network.o DB.o mysql_connection_yk.o \
		mysql_recordset_yk.o DataPool.o
	g++ -o PZY_SVR main.o \
		Network.o \
		Log.o DB.o \
		mysql_connection_yk.o \
		mysql_recordset_yk.o \
		DataPool.o \
		--std=c++0x -lpthread -lmysqlclient

main.o: Paizhaoyi_Project/main.cpp Paizhaoyi_Log/Log.h \
		Paizhaoyi_Network/Network.h
	g++ -c Paizhaoyi_Project/main.cpp

Log.o: Paizhaoyi_Log/Log.cpp
	g++ -c Paizhaoyi_Log/Log.cpp

Network.o: Paizhaoyi_Network/Network.cpp Paizhaoyi_Log/Log.h \
		Paizhaoyi_NetProtocal/NetProtocal.h \
		Paizhaoyi_DBProtocal/DBProtocal.h
	g++ -c Paizhaoyi_Network/Network.cpp Paizhaoyi_Log/Log.cpp \
		-lpthread -lmysqlclient -I /usr/include/mysql

DB.o: Paizhaoyi_Model/DB.cpp Paizhaoyi_Model/mysql_connection_yk.h \
		Paizhaoyi_Model/DataPool.h \
		Paizhaoyi_DBProtocal/DBProtocal.h
	g++ -c Paizhaoyi_Model/DB.cpp \
		Paizhaoyi_Model/mysql_connection_yk.cpp \
		Paizhaoyi_Model/DataPool.cpp \
		-lpthread -lmysqlclient -I/usr/include/mysql

mysql_connection_yk.o: Paizhaoyi_Model/mysql_connection_yk.cpp \
		Paizhaoyi_Model/mysql_recordset_yk.h
	g++ -c Paizhaoyi_Model/mysql_connection_yk.cpp \
		Paizhaoyi_Model/mysql_resordset_yk.cpp
		-lmysqlclient -I /usr/include/mysql/

mysql_recordset_yk.o: Paizhaoyi_Model/mysql_recordset_yk.cpp 
	g++ -c Paizhaoyi_Model/mysql_recordset_yk.cpp \
		-lmysqlclient -I /usr/include/mysql

DataPool.o: Paizhaoyi_Model/DataPool.cpp \
		Paizhaoyi_Model/mysql_connection_yk.h \
		Paizhaoyi_DBProtocal/DBProtocal.h 
	g++ -c Paizhaoyi_Model/DataPool.cpp \
		Paizhaoyi_Model/mysql_connection_yk.cpp \
		-lpthread -lmysqlclient -I /usr/include/mysql/

clean: 
	rm *.o