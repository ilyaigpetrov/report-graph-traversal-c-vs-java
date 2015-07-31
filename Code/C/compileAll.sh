dirlist=$(find ./Inputs -mindepth 1 -maxdepth 1 -type d)
for dir in $dirlist
do
  echo $dir
  if [ ! -f $dir/Expression.h ]; then
      echo "Supply stopping expression in $dir/Expression.h file! See README for details."
      exit 1
  fi
  gcc BFS.c -O3 -o $dir/bfs.exe -include $dir/Expression.h || exit 1
  gcc DFS.c -O3 -o $dir/dfs.exe -include $dir/Expression.h || exit 1
done
