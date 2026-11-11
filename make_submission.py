#!/usr/bin/env python3
from pathlib import Path
from zipfile import ZipFile, ZIP_DEFLATED
root = Path(__file__).resolve().parent
required = ['src/GraphStorage.cpp','src/Graph.hpp','src/Graph.cpp','src/DisjointSet.hpp','src/DisjointSet.cpp','src/graph_tool.cpp','test/test_graph.cpp','test/test_dsu.cpp']
for name in required:
    if not (root/name).is_file(): raise SystemExit('Missing required file: '+name)
files = sorted(p for folder in ['src','test'] for p in (root/folder).rglob('*') if p.is_file() and p.suffix in {'.cpp','.h','.hpp'})
if any(p.is_symlink() for p in files): raise SystemExit('Symlinks are not allowed')
with ZipFile(root/'submission.zip','w',ZIP_DEFLATED) as z:
    for p in files: z.write(p,p.relative_to(root))
print('Created submission.zip with '+str(len(files))+' source/test files.')
