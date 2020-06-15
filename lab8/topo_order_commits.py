# Keep the function signature,
# but replace its body with your implementation
#
# Note that this is the driver function.
# Please write a well-structured implemention by creating other functions
# each of which has a designated purpose.
#
# As a good programming practice,
# please do not use any script-level variables that are modifiable.
# This is because those variables live on forever once the script is imported,
# and the changes to them will persist across different invocations of the imported functions
import sys
import os
import zlib

class CommitNode:
    def __init__(self, commit_hash):
        """
        :type commit_hash: str
        """
        self.commit_hash = commit_hash
        self.parents = list() #set()
        self.children = list() #set()

def find_parents(str_obj):
    parent_ind = 0
    par_hashes = []
    more_to_find = True;
    start = 0
    end = str_obj.find("author", start)
    while more_to_find:
        parent_ind = str_obj.find("parent ", start)
        if parent_ind == -1:
            break
        hash_start = parent_ind + 7
        for k in range(hash_start, end):#len(str_obj)):
            if str_obj[k] == '\n':
                hash_end = k - 1
                break
        par_hashes.append(str_obj[hash_start:hash_end + 1])
        start = parent_ind + 7
    return par_hashes

###########

def get_topo_ordered_commits(root_vertices, found):
    order = []
    # visited is the union of the gray and black vertices  
    visited = set()
    gray_stack = []
    stack = list(root_vertices)
 
    while stack:
        v = stack.pop()
        if v in visited:
            continue
        visited.add(v)

        #v is not a child of the vertex on the top of the gray stack
        while (gray_stack and v not in found[gray_stack[-1]].children):
            g = gray_stack.pop()
            order.append(g)

        gray_stack.append(v)
        for c in found[v].children:
            if c in visited:
                continue
            stack.append(c)
    
    while (gray_stack):
        order.append(gray_stack.pop())
    
    return order

###########

def print_topo_ordered_commits(ordered, root_commits_print, found):
    com_curr = ""
    com_next = ""

    for i in range(0, len(ordered) - 1): #problem?
        com_curr = ordered[i]
        com_next = ordered[i + 1]

        print(com_curr, end=(''))

        if com_curr in root_commits_print:
            print(" " + root_commits_print[com_curr])
        else:
            print("")

        if not com_next in found[com_curr].parents:
            count = 0
            for parent in found[com_curr].parents:
                if count == len(found[com_curr].parents) - 1:
                    print(parent, end=(""))
                else:
                    print(parent, end=(" "))
                count += 1
            print("=\n\n=", end=(''))
            for child in found[com_next].children:
                print(child, end=(" "))
            print("\n", end=(''))
#            for j in range(0, len(found[com_curr].parents)):
#                if j == len(found[com_curr].parents) - 1:
#                    print(found[com_curr].parents[j])
#                else:
#                    print(found[com_curr].parents[j] + "\n")
#            print("=\n\n=")
          #  for child in found[com_curr].children:
           #     print(child + " ", end=(''))
    if ordered:
        print(ordered[-1] + " ", end='')
        if ordered[-1] in root_commits_print:
            print(root_commits_print[ordered[-1]], end='')
        print()

###########

def get_loc_branches(dir, loc_br_aliases, local_branches):
#    print('DEBUG ', dir)
    with os.scandir(dir) as it:
        for item in it:
            if item.is_dir():
#                print('DEbug ', item.path)
                get_loc_branches(item.path, loc_br_aliases, local_branches)
            elif item.is_file():
                loc_br = os.path.relpath(item.path, local_branches) 
               # print(loc_br + ", " + local_branches)
                #    ind = loc_br.find("heads/") + 6
            #    loc_br = loc_br[ind:]
                loc_br_aliases.append(loc_br)


###########

def topo_order_commits():
    toplevel = ""
    cwd_list = os.listdir()
    for i in range(0, len(cwd_list)):
        if cwd_list[i] == ".git":
            toplevel = os.getcwd()
            break

    abs_path = os.getcwd()
    while toplevel == "":
        ind = abs_path.rfind("/")
        abs_path = abs_path[0:ind]
        if not os.path.isdir(abs_path):
            sys.stderr.write("Not inside a Git repository\n")
            exit(1)
        abs_path_list = os.listdir(abs_path)
        for k in range(0, len(abs_path_list)):
            if abs_path_list[k] == ".git":
                toplevel = abs_path

    local_branches = toplevel + "/.git/refs/heads"
    
    loc_br_aliases = list()
    get_loc_branches(local_branches, loc_br_aliases, local_branches) 
#    loc_br_aliases = os.listdir(local_branches)
#    print(loc_br_aliases)

#THE PLAN: Use DFS using stack in order to find all commits and turn into 
# Commit Nodes, then iterate through list and open objects files to add 
#parents/children of each Commit Node 

    stack = []
    found = {}
    root_commits_print = {}
    root_commits = []

    for loc_branch in loc_br_aliases:
        branch_file = open(local_branches + "/" + loc_branch, 'r')
        br_hash = branch_file.readline().rstrip()
        stack.append(br_hash)
        if not br_hash in found:
            found[br_hash] = CommitNode(br_hash)
        if not br_hash in root_commits_print:
            root_commits_print[br_hash] = loc_branch
        else:
            root_commits_print[br_hash] = root_commits_print[br_hash] + " " + loc_branch
   
#print(stack)
    visited = set()

    while stack:
        par_branch = stack.pop()
        if par_branch in visited:
            continue
        visited.add(par_branch)
     #  print(par_branch)
        abs_path = toplevel + "/.git/objects/" + par_branch[0:2] + "/" + par_branch[2:len(par_branch)]
        ch_commit = open(abs_path, "rb").read()
        ch_commit = zlib.decompress(ch_commit)
        ch_commit = str(ch_commit, 'utf-8')
        print(ch_commit)
    
        par_hashes = find_parents(ch_commit)
     #  print(par_hashes)
    
        if (len(par_hashes) == 0):
           root_commits.append(par_branch)
         # root_commits[par_branch] = CommitNode(par_branch)

        for m in range(0, len(par_hashes)):
            if not par_hashes[m] in found:
                found[par_hashes[m]] = CommitNode(par_hashes[m])
                stack.append(par_hashes[m])

            parent = found[par_hashes[m]]

            if par_branch not in parent.children:
                parent.children.append(par_branch)
            if par_hashes[m] not in found[par_branch].parents:
                found[par_branch].parents.append(par_hashes[m])

           # parent.children.append(par_branch)
           # found[par_branch].parents.append(par_hashes[m])
           # parent.children.add(par_branch)
           # found[par_branch].parents.add(par_hashes[m])

        for key in found:
            found[key].children.reverse()

#    for val in found:
#        print("Hash: " + val)
#        print("Parents: ")
#        print(found[val].parents)
#        print("Children: ")
#        print(found[val].children)

    ordered = get_topo_ordered_commits(root_commits, found)
#    print("Root_commits: ")
#    print(root_commits_print)
#     print(ordered)

    print_topo_ordered_commits(ordered, root_commits_print, found)


if __name__ == '__main__':
    topo_order_commits()
