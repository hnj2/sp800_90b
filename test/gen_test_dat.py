import pypuf.io
import numpy as np

def challenges_sequence(n, N=10**6, seed=0):
    d = np.fromiter(range(seed, seed + N), dtype=int)
    return 1 - 2 * (((d[:,None] & (1 << np.arange(n)))) > 0).astype(np.int8)
    
def challenges_random(n, N=10**6, seed=31415):
    return pypuf.io.random_inputs(n, N, seed)

def generate(puf, challenges, file):
    responses = (.5 - .5 * puf.eval(challenges)).astype(np.int8)
    with open(file, 'wb') as f:
        f.write(responses)

import pypuf.simulation.delay

PUFS = {
    'xor-arbiter-puf.64-4': pypuf.simulation.delay.XORArbiterPUF(n=64, k=4, seed=5),
}
SEQS = {
    'seq': challenges_sequence,
    'random': challenges_random,
}

for puf_name, puf in PUFS.items():
    for seq_name, seq in SEQS.items():
        generate(
            puf=puf, challenges=seq(n=puf.challenge_length, N=10**6),
            file=f'{puf_name}.{seq_name}.dat',
        )
