#!/usr/bin/python

from urllib import urlencode, urlopen
from sys import argv
from uuid import uuid4
from MySQLdb import connect

def Request(id):
        param = urlencode({"id": id})
        urlopen("http://192.168.17.22:8081/create.php?%s" % param)
        print 'done'


if __name__ == '__main__':
    if len(argv) != 2:
        print 'Usage: %s <id>' % argv[0]
    else:
	conn = connect(db="pzy", user="root", host="localhost")
	c = conn.cursor()

	c.execute("update qrtest set guid=%s where id=%s", (str(uuid4()), int(argv[1])))
	conn.commit()
	conn.close()
	Request(int(argv[1]))
	
