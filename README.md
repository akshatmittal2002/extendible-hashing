# Extendible-Hashing
<br>

### Input:
First two lines describe the initialization parameters for your extendible hash table.<br>
`Line 1`: Global depth<br>
`Line 2`: Bucket capacity<br>
From `Line 3` onwards, the operations to be performed on the hash table are described:<br>
&emsp;`2`: Insert new value<br>
&emsp;`3`: Search a value<br>
&emsp;`4`: Delete a value<br>
&emsp;`5`: Display status of the hash table<br>
&emsp;`6`: Quit<br><br>

### Output:
For operation `5`, output will be:<br>
`Line 1`: Value of Global Depth<br>
`Line 2`: Number of buckets<br>
From `Line 3` onwards, list of pairs <number of keys, local depth> for each bucket.<br>
Buckets should be ordered in the `ascending order of creation time`.<br><br>

```
Note:
1. Directory can grow only up to the Global Depth of 20.
2. Deletions to be handled in a lazy manner. Only delete the corresponding key.
```