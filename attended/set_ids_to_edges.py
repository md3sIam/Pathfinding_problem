file_read = open("../maps/open_test/NY_roads.csv", "r")
file_write = open("../maps/open_test/Toronto_id_roads.csv", "w")

current_id = 0
first_line = True
for line in file_read:
    if first_line:
        file_write.write('id, ' + line)
        first_line = False
    else:
        file_write.write(str(current_id) + ', ' + line)
        current_id += 1

file_read.close()
file_write.close()
