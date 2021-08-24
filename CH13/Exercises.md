# Practice Exercises

1. Some systems automatically delete all user files when a user logs off or
a job terminates, unless the user explicitly requests that they be kept.
Other systems keep all files unless the user explicitly deletes them.
Discuss the relative merits of each approach

Systems that delete files that are not explicitly saved have the advantage of continuously removing unnecessary files.
This saves space of secondary storage. The great disadvantage is that important files may be accidentally deleted.

Systems that implicitly save all files are more relevant today since disk storage is not as expensive per byte.
It also gives better flexibility since users can delete files on-demand when the secondary storage run out of space.

2. Why do some systems keep track of the type of a file, while still others
leave it to the user and others simply do not implement multiple file
types? Which system is “better”?

If the OS keeps track of the file, it can provide different operations of each type. For example, if a file is a serialized portion of a database table, it can be read in blocks, whereas an ASCII file could only be read on a byte basis.

Other systems that do not keep track of the type of file leave it to user-space applications to identify the file. This makes the OS simpler, more regular, and more performant. 

The better approach is the one that provides relevant features for user-space processes. If processes would do better with OS support, then the first type of systems would be more appropriate. For a more general-purpose system, where user-space support a variety of files, then the second type of systems would be better.

4. Could you simulate a multilevel directory structure with a single-level
directory structure in which arbitrarily long names can be used? If your
answer is yes, explain how you can do so, and contrast this scheme with
the multilevel directory scheme. If your answer is no, explain what
prevents your simulation’s success. How would your answer change
if file names were limited to seven characters?

Prefixes could be added to the name itself, e.g. `user1_subdir1_file1.c`. However, this would not be possible if the file name is limited to a small number of chars. Another approach is to use a map from long file names, e.g. `user1_subdir1_file1.c` to a 7-char name and store this map in a file.

5. Explain the purpose of the open() and close() operations

`open()`: A one-time operation that checks the privileges of the process against the file permissions and checks the validity of the name, and returns back a handle which can be used in subsequent operations. This is better than doing all this work every time an operation is to be done on the file.

`close()`: Removes the file from the per-process open-file table, and subsequently frees a place for another file and a descriptor. Also, if the OS caches edits, `close()` makes sure that those changes are committed to the secondary storage.

6. In some systems, a subdirectory can be read and written by an authorized user, just as ordinary files can be.

a. Describe the protection problems that could arise.  
b. Suggest a scheme for dealing with each of these protection problems.

Sub directory entries are not supposed to keep data, they only contain pointers to other files and subdirs that are inside it. There's little to no sense to enable the user to edit those entries. If a subdir is writable, users may be able to add files to which they do not have access from other locations to this subdir.

Completely disabling writing to a subdirectory entry is a suitable solution to this problem.

7. Consider a system that supports 5,000 users. Suppose that you want to
allow 4,990 of these users to be able to access one file.

a. How would you specify this protection scheme in UNIX?  
b. Can you suggest another protection scheme that can be used more
effectively for this purpose than the scheme provided by UNIX?

To put the 4,990 users inside a group along with the file owner, and enable access for the group.  
Add the 4990 users to the _Access Control List_ of the file.

Another solution: Put the remaining 10 users in the ACL and disable their access, and enable the access universally for the file.
Since ACL rules have higher priority, its rules _and_ the universal rule will both apply leaving the 4990 users in control of the file, and the other 10 cannot access it.