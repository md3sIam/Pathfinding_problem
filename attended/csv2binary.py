import struct


nodes_filename = "../maps/open_test/SPb3_nodes.csv"
edges_filename = "../maps/open_test/SPb3_id_roads.csv"
record_filename = "../maps/binaries/sbp3_test.graph"


file_record = open(record_filename, "wb")

# record nodes
file_nodes = open(nodes_filename, "r")
first_line = True
for line in file_nodes:
    if first_line:
        first_line = False
    else:
        parsed = line.split(',')
        file_record.write(struct.pack('ldd',
                                      int(parsed[0]),
                                      float(parsed[1]),
                                      float(parsed[2])))
        print(int(parsed[0]),
              float(parsed[1]),
              float(parsed[2]))
file_nodes.close()
print("Nodes done...")

# separator between vertices and edges
file_record.write(struct.pack('ldd', 0, 0, 0))

# record edges
file_edges = open(edges_filename, 'r')
first_line = True
for line in file_edges:
    if first_line:
        first_line = False
    else:
        parsed = line.split(',')
        file_record.write(struct.pack('llld',
                                      int(parsed[0]),
                                      int(parsed[1]),
                                      int(parsed[2]),
                                      float(parsed[3])))
file_edges.close()
print("Edges done")

# separator between edges and future possible info
file_record.write(struct.pack('llld', 0, 0, 0, 0))

file_record.close()

# -----------
# just check
file_check = open(record_filename, 'rb')

while True:
    pack = file_check.read(24)
    id, lon, lat = struct.unpack('ldd', pack)
    if id == 0 and lon == 0 and lat == 0:
        break

while True:
    pack = file_check.read(32)
    id, idf, idt, weight = struct.unpack('llld', pack)
    print(weight)
    print(bin(struct.unpack('l', struct.pack('d', weight))[0]))
    if id == 0 and idf == 0 and idt == 0 and weight == 0:
        break

file_check.close()
print("file check done!")
