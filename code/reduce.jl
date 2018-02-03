using MPI
using Base.Test

const  N = 1024*1024*100

MPI.Init()
rank = MPI.Comm_rank(MPI.COMM_WORLD)
size = MPI.Comm_size(MPI.COMM_WORLD)

const  myN = N ÷ size
const array = fill(rank+1.0,myN)

function mpi_sum(a)
    mysum = 0.0
    for x in a
        mysum += x
    end
    return MPI.Reduce(mysum, +, 0, MPI.COMM_WORLD)
end

expected = sum(myN*(1:size))
for i in 1:3
    rank == 0 ? (@time @test expected == mpi_sum(array)) : mpi_sum(array)
end

MPI.Finalize()
