#!/bin/env python3

import sys
import os

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '')))

# modules we depend on
from utils.mips import Mips
from utils.pnach import PnachWriter
from utils.elf import ElfWrapper
from utils.ldparse import LdScript

# Yields word chunks from a word-aligned byte array
def yieldWordChunksFromBytes(byteArray: bytes):
	length = len(byteArray)
	if length % 4 != 0:
		print(f'WARNING: Core is not word-aligned?')
	counter = 0
	while True:
		if counter >= length:
			break
		chunk = byteArray[counter:counter+4]
		if len(chunk) != 4:
			#print(f'a {len(chunk)}')
			#print(chunk)
			break
		yield chunk
		counter += 4

# The fun begins...
def main():
	# Open the ELF file so we can get at symbols in it.
	# Open the Region-specific LD file so we can grab other symbols we need too
	elf = ElfWrapper(f'bin/ssx_og_chaos_{sys.argv[2]}.elf')
	ldScript = LdScript.new(f'bxapi/ld/{sys.argv[2]}.ld')

	# Write the pnach out.
	pnachAuthor = 'modeco80'
	pnachComment = 'SSX Chaos Mod (game side)'

	with PnachWriter.file(sys.argv[1]) as pnachWriter:
			with pnachWriter.cheat('SSX Chaos Mod', pnachAuthor, pnachComment) as cheat:
				def jHook(hookAddress: int, targetAddress: int):
					cheat.setAddress(hookAddress)
					cheat.word(Mips.j(targetAddress))

				# Hook to the start
				jHook(ldScript.symbol('bxInitREAL_HookPoint'), elf.symbol('_start'))

				# For our last step, poke in the code blob.
				cheat.comment(' Code Blob Begin')
				cheat.setAddress(elf.symbol('_begin'))
				for word in yieldWordChunksFromBytes(elf.makeCore()):
						cheat.word(word)
				cheat.comment(' Code Blob End')

if __name__ == '__main__':
	main()
