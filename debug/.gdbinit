set confirm off
set print thread-events off
set host-charset UTF-8
set history save on
set history filename ~/.gdb_history
define asm
  layout asm
  layout reg
end
define src
  layout src
  layout reg
end
src