#
# Thomas Buffard 
# Simple program to parse vertex data from .obj format
# to manually make into a C++ array
#

with open('/Users/thomas/Desktop/zero.obj') as file:
    file_content = file.read()

flatten = lambda l: [item for sublist in l for item in sublist]


lines = ([ [ element for element in line.split(" ") ] for line in file_content.splitlines() ])

vertices = flatten([ [ x for x in line if x.replace('.','',1).lstrip('-').isdigit() ] for line in lines if len(line) == 4 and line[0] == 'v' ])
vertices_string = ' '.join(vertices)
print(vertices_string);

elements = flatten([ [ x.split('/')[0] for x in line if x != 'f'] for line in lines if len(line) == 4 and line[0] == 'f' ])
elements_string = ' '.join(elements)
print(elements_string)

