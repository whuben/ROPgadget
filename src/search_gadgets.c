/*
** RopGadget - Release v3.4.2
** Jonathan Salwan - http://twitter.com/JonathanSalwan
** Allan Wirth - http://allanwirth.com/
** http://shell-storm.org
** 2012-11-11
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "ropgadget.h"

void search_gadgets(unsigned char *data, unsigned int size_data)
{
  t_map   *maps_exec;
  t_map   *maps_read;
  unsigned int NbGadFound = 0;
  unsigned int NbTotalGadFound = 0;

  if (asm_mode.flag)
    {
      if (containerType == CONTAINER_ELF32)
        x8632_build_code(asm_mode.argument);
      else
        {
          fprintf(stderr, "Assembly building mode not available for this architecture.\n");
          return;
        }
    }

  save_section();

  maps_exec = return_map(0);
  maps_read = return_map(1);

  fprintf(stdout, "%sGadgets information\n", YELLOW);
  fprintf(stdout, "============================================================%s\n", ENDC);

  /* Linux/x86-32bits & FreeBSD/x86-32bits*/
  if (containerType == CONTAINER_ELF32)
    find_all_gadgets(data, size_data, maps_exec, maps_read, tab_x8632, &NbGadFound, &NbTotalGadFound);
  else if (containerType == CONTAINER_ELF64)
    find_all_gadgets(data, size_data, maps_exec, maps_read, tab_x8664, &NbGadFound, &NbTotalGadFound);
  else
    {
      fprintf(stderr, "Gadget searching not supported for this architecture.\n");
      return;
    }

  if (opcode_mode.flag != 1 && stringmode.flag != 1)
    {
      fprintf(stdout, "\n\n%sPossible combinations.\n", YELLOW);
      fprintf(stdout, "============================================================%s\n\n", ENDC);
      if (containerType == CONTAINER_ELF32)
        x8632_ropmaker();
      else if (containerType == CONTAINER_ELF64)
        x8664_ropmaker();
      else
        {
          fprintf(stderr, "Ropmaking not supported for this architecture.\n");
          return;
        }
    }

  free_add_map(maps_exec);
  free_add_map(maps_read);

  if (opcode_mode.flag == 1)
    fprintf(stdout, "\nTotal opcodes found: %s%d%s\n", YELLOW, NbTotalGadFound, ENDC);
  else if (stringmode.flag == 1)
    fprintf(stdout, "\nTotal strings found: %s%d%s\n", YELLOW, NbTotalGadFound, ENDC);
  else
    fprintf(stdout, "\nUnique gadgets found: %s%d%s\n", YELLOW, NbGadFound, ENDC);
}
