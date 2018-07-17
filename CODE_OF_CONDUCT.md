# Style guide 

## Astyle

### 1) Install astyle
On Ubuntu
```
$ sudo apt-get install astyle
```

### 2) Create astylerc in your home path
On Ubuntu 
```
$ vim ~/astylerc
```
And add:
```
--style=linux
--indent-namespaces
--indent=spaces=4
--indent-switches
--indent-col1-comments
--break-blocks
--delete-empty-lines
--convert-tabs
--max-code-length=200
--indent-preproc-define
--indent-preproc-cond
--indent-preproc-block
--break-blocks
--min-conditional-indent=0
--unpad-paren
```

### 3) Use astyle
On Ubuntu
```
$ astyle -r -n "*.h" "*.c" --options=~/astylerc
```
