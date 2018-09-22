# HilMa
Hilbert Mapper is a C++ implementation of Hilbert's space filling curve to represent read coverage on genomic sequences. Although it can be used to represent any unidimensional vector of positive numerical data.
## Usages
reading a samtools depth output file:

```python hilma.py sam [coverage file from samtools depth] [contig ID]```

awk, grep, custom script, ... outputting integer unidimensional data to stdout:

```[stdin] | python hilma.py vector - [output prefix]```

reading a custom coverage file:

```python hilma.py custom [column of IDs] [column of values] [coverage file] [ID of sequence to plot]```

This usage may change in future commits!

## Exemples
```
samtools depth mybam.bam > mybam.cov
python hilma.py mybam.cov contig3
grep contig3 mybam.cov | python hilma.py vector - ctg3
python hilma.py custom.cov 0 3 contig_002
```
