import math
import sys
import re
import graph

class street: # the street data is stored as an object
    total_number = 0
    total_instances = []
    
    def __init__(self, name, coordinates):
        self.name = name
        street.total_number += 1
        self.coordinates = coordinates
        self.lines = []
        for j in coordinates:
            if coordinates.index(j) < len(coordinates)-1:
                self.lines.append([j,coordinates[(coordinates.index(j)+1)]])
        street.total_instances.append(self)
    
    def get(self):
        return self.name, self.coordinates
    
    def remove(self):
        street.total_instances.remove(self)
        del self

    @classmethod
    def instantiate(cls, call_name, coordinates):
        call_name = street(call_name, coordinates)

    def __repr__(self):
        return f"[{self.name}, {self.lines}]"

def check_coordinates(line1):
    nedge = 0
    start_edge = 0  

    if bool(re.search(r"\)\(", line1)) == True or bool(re.search(r"\)\s*\,\s*\(", line1)) == True: 
        raise Exception("Coordinate formatting is incorrect") # checking for no space between coordinates of comma between coordinate set

    if bool(re.search(r"\(\s*\(+", line1)) == True or bool(re.search(r"\)\s*\)+", line1)) == True:
        raise Exception("Coordinate formatting is incorrect") # checking for more than one same consecutive brackets

    if bool(re.search(r"\-\s*\-+", line1)) == True:
        raise Exception("Coordinate formatting is incorrect") # checking for more than one negative sign

    if bool(re.search(r"\)\s*[a-zA-Z0-9]+", line1)) == True or bool(re.search(r"[a-zA-Z]+\s*\)", line1)) == True:
        raise Exception("Coordinate formatting is incorrect") # checking for alphabets with open bracket. In some case numbers were also considered 

    if bool(re.search(r"\(\s*[a-zA-Z]+", line1)) == True or bool(re.search(r"[a-zA-Z0-9]+\s*\(", line1)) == True:
        raise Exception("Coordinate formatting is incorrect") # checking for alphabets with open bracket. In some case numbers were also considered 

    if bool(re.search(r"\)\s*[@''`.,_!#$%^&*<>?/\|}{~:-]+", line1)) == True or bool(re.search(r"[@''`.,_!#$%^&*<>?/\|}{~:-]+\s*\)", line1)) == True:
        raise Exception("Coordinate formatting is incorrect") # checking for special characters with close brackets

    if bool(re.search(r"\(\s*[@''`.,_!#$%^&*<>?/\|}{~:-]+", line1)) == True or bool(re.search(r"[@''`.,_!#$%^&*<>?/\|}{~:-]+\s*\(", line1)) == True:
        raise Exception("Coordinate formatting is incorrect") # checking for special characters with open brackets

    if bool(re.search(r"\(\s*\)", line1)) == True:
        raise Exception("Coordinate formatting is incorrect") # checking for empty coordinate set

    # if bool(re.search(r"\s*\(\s*\-{0,1}\d*\.\d*\s*\,\s*\-{0,1}\d*\.\d*\s*\(", line1)) == True:
    #     raise Exception("coordinate close bracket not provided")


def check_command(cmd): 
    if cmd != 'add' and cmd != 'gg' and cmd != 'rm' and cmd != 'mod':
        raise Exception("Improper command")
        
def parse(line):   
    diff = line.strip().split()
    if len(diff) == 0:
        raise Exception("No input provided")

    check_coordinates(line)
    check_command(diff[0])
    return diff


def gen_coordinates(line):
    if bool(re.search(r"\+", line)) == True:
        raise Exception("+ sign is not allowed in the coordinates")
    
    coordinates = re.findall(r"\s*\(*\s*\-*\s*\d\.*\d*\s*\,*\s*\-*\s*\d*\.*\d*\s*\)*",line)

    cord = []
    for c in coordinates:
        if bool(re.search(r"\-\s+\d",c)) == True:
            raise Exception("No space between - and number allowed")

        else:
            cord.append(eval(re.sub(r"\s+", "",c)))
    
    for c in range(len(cord)):
        if len(cord[c]) > 2:
            raise Exception("Coordinates can only have 2 axes")

    return cord

def vertex_print(ar=[]): # it will print the vertex as a string to two decimal places
    sys.stdout.write("V = {" + "\n")
    for i in range(len(ar)):
        sys.stdout.write(f"  {i+1}: ({str(round(ar[i][0],2))},{str(round(ar[i][1],2))})" + "\n")

    sys.stdout.write("}" + "\n")

def edge_print(ar=[]): # it will print the edge list by using the order list of the vertex
    
    sys.stdout.write("E = {" + "\n")
    for i in range(0, len(ar)-1):
        tmp_str = '  <'+str(ar[i][0])+','+str(ar[i][1])+'>,'
        sys.stdout.write(tmp_str + "\n")
    if len(ar) != 0:
        i = len(ar)-1
        tmp_str = '  <'+str(ar[i][0])+','+str(ar[i][1])+'>'
        sys.stdout.write(tmp_str + "\n")
    sys.stdout.write('}\n')    

def check_name(array_list,line): # this is used for the "add" and "mod" command to check the proper pattern
    if array_list[1].find('"') == 0: # the name of the street must start with "
        pass
    else:
        raise Exception("Formatting is not correct")

    if bool(re.search(r'\s+\".+\"\s+',line)) == True: # check for the street name formatting
        street_name = re.search(r'\".+\"',line).group()
    else:
        raise Exception("Either street name not provided or not properly formatted")

    if bool(re.search("[@''`.,_!#$%^&*()<>?/\|}{~:-]",street_name)) == True: 
        raise Exception("Street name contains special characters") # check for the street name having any special characters and coordinates have proper numbers
    
    if bool(re.search("\d+",street_name)) == True:
        raise Exception("Street name contains number(s)")

    if street_name.count('"') > 2:
        raise Exception("Street name formatting is incorrect")

    street_name = str(re.search(r'\"\D+\"',line).group()) # stores the street name within the double quotes with the exact whitespaces

    return street_name

def street_name_check(line,array_list): # used for rm command
    
    if array_list[1].find('"') == 0: # the name of the street must start with "
        pass
    else:
        raise Exception("Formatting is not correct")

    if bool(re.search(r'\".+\"\s*$',line)) == True: # check for the street name formatting
        street_name = re.search(r'\".+\"',line).group()
    else:
        raise Exception("Either street name not provided or not properly formatted")

    if line.count('"') > 2:
        raise Exception("Formatting is incorrect")

    if bool(re.search("[@''`.,_!#$%^&*()<>?/\|}{~:-]",line)) == False and bool(re.search("\d+",line)) == False: 
        return street_name # check for the street name having any special characters and coordinates have proper numbers
    else:
        raise Exception("Street name contains special characters or number")

def main():
    list = []
    while True:
        line = sys.stdin.readline()      

        if line == "":
            break

        else:
            try:
                st = None
                array_list = parse(line)
                
                #street_name = " ".join(array_list[1:start_edge])

                if array_list[0] == "add":
                    street_name = check_name(array_list , line)
                    cord = gen_coordinates(line)
                    if street_name.isnumeric() == True:
                        raise Exception("Street Name not provided or is only numeric")
                    if cord == []:
                        raise Exception("Coordinates not provided")
                    
                    for object in street.total_instances:
                        if object.name == street_name:
                            raise Exception("Cannot add the same street again")                    
                    street.instantiate(street_name,cord)
                    
                if array_list[0] == "rm":
                    street_name = street_name_check(line,array_list)
                    cord = gen_coordinates(line)
                    if street_name == "":
                        raise Exception("Street name not provided")
                    
                    check = 0
                    for object in street.total_instances:
                        if object.name.casefold() == street_name.casefold():
                            object.remove()
                            check += 1

                    if check == 0:
                        raise Exception("Street does not exist in database")

                if array_list[0] == "mod":
                    street_name = check_name(array_list, line)
                    cord = gen_coordinates(line)
                    if cord == [] and street_name == "":
                        raise Exception("Street name and coordinates not provided")
                    
                    if cord == [] or street_name == "":
                        raise Exception("Coordinates or street name not provided")
                    
                    check = 0
                    for a in street.total_instances:
                        if a.name.casefold() == street_name.casefold():
                            a.coordinates = cord
                            a.lines = []
                            for j in cord:
                                if cord.index(j) < len(cord)-1:
                                    a.lines.append([j,cord[(cord.index(j)+1)]])
                            check += 1
                    
                    if check == 0:
                        raise Exception("Street does not exist in database")
        
                if array_list[0] == "gg":
                    ver =[]
                    inter =[]
                    for i in range(len(street.total_instances)):
                        for j in range(i+1,len(street.total_instances)):
                            graph.compare_line(street.total_instances[i].lines,street.total_instances[j].lines,ver,inter)

                    for m in range(len(ver)):
                        for n in reversed(range(m+1,len(ver))):
                            if ver[m][0] == ver[n][0] and ver[m][1] == ver[n][1]:
                                ver.pop(n)

                    edge_list = graph.generate_edge(street.total_instances,inter,ver)
                    vertex_print(ver)
                    edge_print(edge_list)

                #print(street.total_instances)          
            except Exception as exc:
                sys.stdout.write("Error: " + str(exc) +"\n")
                
                # exc_type, exc_obj, exc_tb = sys.exc_info()
                # print(exc_type, exc_tb.tb_lineno)

            sys.stdout.flush()

        #print(line, file=sys.stdout) # the "file" is for unix systems
    sys.exit(0)
     # this terminates everything.
    

if __name__ == "__main__":
    main()