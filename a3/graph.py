import sys
import math
import re

def distance(p1,p2):
    return math.sqrt((p1[0] - p2[0])**2 + (p1[1] - p2[1])**2)

def point_on_line(line_coord1, point, line_coord2):
    if distance(line_coord1,point) + distance(point,line_coord2) - distance(line_coord1,line_coord2) < 0.00000000001:
        return True
    
    else:
        return False

def intersect(l1, l2):
    x1, y1 = l1[0][0], l1[0][1] 
    x2, y2 = l1[1][0], l1[1][1]
    x3, y3 = l2[0][0], l2[0][1] 
    x4, y4 = l2[1][0], l2[1][1]
 
    x_list1 = [x1,x2]
    x_list2 = [x3,x4]
    x_list1.sort()
    x_list2.sort()    
 
    y_list1 = [y1,y2]
    y_list2 = [y3,y4]
    y_list1.sort()
    y_list2.sort() 
 
    list_x = [x1,x2,x3,x4]
    list_x.sort()
    list_y = [y1,y2,y3,y4]
    list_y.sort()
    
    intersect_list = []
 
    if x1 != x2 and x3 != x4: 
     
        k1 = (y2 - y1)/(x2 - x1)
        k2 = (y4 - y3)/(x4 - x3)
        
        b1 = y1 - k1*x1
        b2 = y3 - k2*x3
    

    
        if k1!=k2 :
        
            xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4))
            xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4))
            xcoor =  xnum / xden

            ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4)
            yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4)
            ycoor = ynum / yden

            if x_list1[0] <= xcoor <= x_list1[1] and x_list2[0] <= xcoor <= x_list2[1]  and  y_list1[0] <= ycoor <= y_list1[1] and y_list2[0] <= ycoor <= y_list2[1]:
                
                return (xcoor, ycoor)

            else:
              
                return 0
            
        else:
       
            if b1 == b2:

                if (x_list1[1] > x_list2[0] and x_list1[0] < x_list2[0]) or (x_list2[1] > x_list1[0] and x_list2[0] < x_list1[0]):
                    intersect_list.append([list_x[1],list_y[1]])
                    intersect_list.append([list_x[2],list_y[2]])
                    return intersect_list

                elif x_list1[1] == x_list2[0]:
                    return (list_x[1],list_y[1])
                else:
                    return 0

            else:
                return 0
                
            
                
    
    elif x1 != x2 or x3 != x4:
            xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4))
            xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4))
            xcoor =  xnum / xden

            ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4)
            yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4)
            ycoor = ynum / yden
          

            if x_list1[0] <= xcoor <= x_list1[1] and x_list2[0] <= xcoor <= x_list2[1]  and  y_list1[0] <= ycoor <= y_list1[1] and y_list2[0] <= ycoor <= y_list2[1]:
                return (xcoor, ycoor)
            else:
               
                return 0

    else:
        
        if x1 == x2 == x3 == x4:
            if list_y[1] != list_y[2]:
                intersect_list.append((x1,list_y[1]))
                intersect_list.append((x2,list_y[2]))
                return intersect_list
            else:
                return (x1,list_y[1])
        else:
            return 0

def compare_line(list1,list2,ver,inter):
    for i in range(0,len(list1)):
        for j in range(0,len(list2)):
            
            result_inter = intersect(list1[i],list2[j])
            if isinstance(result_inter,tuple):
                    #print("Tuple")
                    ver.append(result_inter)
                    ver.append(list1[i][0])
                    ver.append(list1[i][1])
                    ver.append(list2[j][0])
                    ver.append(list2[j][1])
                    inter.append(result_inter)

                    
            if isinstance(result_inter,list):
                    ver.append(result_inter)
                    #print("List")
                    ver.append(list1[i][0])
                    ver.append(list1[i][1])
                    ver.append(list2[j][0])
                    ver.append(list2[j][1])
                    ver.append(result_inter[0])
                    ver.append(result_inter[1])
                    inter.append(result_inter[0]) 
                    inter.append(result_inter[1])

def create_edge(line_list, inter,ver):
    inter_list = []
    new_line = []                            #save intersects
    #valid vertices including intersects
    flag = 0
    
    for i in range(len(line_list)): # street object
        for j in range(len(line_list[i].lines)): # line list of a street
            for k in range(len(line_list[i].lines[j])): # one line from the list
                for l in range(len(inter)):        
                    if point_on_line(line_list[i].lines[j][0],inter[l],line_list[i].lines[j][1]) == True: 
                        inter_list.append([line_list[i].lines[j][0],inter[l],line_list[i].lines[j][1]])
        
    # for p in range(len(inter)-1):
    #     for q in range (p,len(inter)):
    #         if inter[p] != inter[q]:
    #             new_line.append([inter[p],inter[q]])
    # print(new_line)
    
    
    for a in range(len(inter_list)):
        for b in reversed(range(a+1,len(inter_list))):
            if inter_list[a] == inter_list[b]:
                inter_list.pop(b)

    semi_final = []
    for x in range(len(inter_list)):
        semi_final.append([inter_list[x][0],inter_list[x][1]])
        semi_final.append([inter_list[x][1],inter_list[x][2]])

    final = []
    for c in range(len(semi_final)):
        for d in range(len(ver)):
            if ver[d] == semi_final[c][0]:
                semi_final[c][0] = d + 1 
            
            if ver[d] == semi_final[c][1]:
                semi_final[c][1] = d + 1 
            


    return(semi_final)

def generate_edge(line_list,inter,ver):
    final_list = []
    
    for i in range(len(line_list)): # street object
        for j in range(len(line_list[i].lines)): # line list of a street
            #for k in range(len(line_list[i].lines[j])): # one line from the list
            inter_list = []
            check = 0
            inter_list.append(line_list[i].lines[j][0])
            inter_list.append(line_list[i].lines[j][1])
            for l in range(len(inter)):        
                if point_on_line(line_list[i].lines[j][0],inter[l],line_list[i].lines[j][1]) == True: 
                    inter_list.append(inter[l])
                    check = 1
            
            if check == 0:
                continue
            
            else:
                for m in range(len(inter_list)):
                    for n in list(reversed(range(m+1,len(inter_list)))):
                        if inter_list[m][0] == inter_list[n][0] and inter_list[m][1] == inter_list[n][1]:
                            inter_list.pop(n)
                
                inter_list.sort()

                for w in range(len(inter_list)-1):
                    final_list.append([inter_list[w],inter_list[w+1]])
                    w += 1

        converted_list = []

        for c in range(len(final_list)):
            for d in range(len(ver)):
                if ver[d] == final_list[c][0]:
                    final_list[c][0] = d + 1 
            
                if ver[d] == final_list[c][1]:
                    final_list[c][1] = d + 1 
    return final_list


    # https://github.com/Youlina3/Traffic-Camera-Installation-Based-on-Vertex-Cover-Optimization/blob/master/parser.py
    # the calculation methodology was used from here ONLY