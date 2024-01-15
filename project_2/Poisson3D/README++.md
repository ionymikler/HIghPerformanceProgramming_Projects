To run an example, do the following, form root
```bash
make
make run
```
This will run the program with default parameters, defined in the Makefile.
The default executable is `possion_gs`, but can be changed to `possion_jacobi`.
Change `verbose` to 1 to print more information.

There are 2 version of each algo: `<algo-name>_seq.c` and `<algo-name>_paral.c`. To compile one or the other, copy it to `<algo-name>.c` and run `make` again.

To generate results run
```bash
make
./testing/job_script.sh
```