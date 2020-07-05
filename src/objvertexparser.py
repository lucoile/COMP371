#
# Thomas Buffard 
# Simple program to parse vertex data from .obj format
# to manually make into a C++ array
#

with open('/Users/thomas/Desktop/zero.txt') as file:
    file_content = file.read()

flatten = lambda l: [item for sublist in l for item in sublist]
vertex_data = flatten([ [ x for x in line.split() if x != 'v' ] for line in file_content.splitlines() ])

print(", ".join(vertex_data))
print(len(vertex_data))