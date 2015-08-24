sub'r{$==rand pop}$|=print"\e[2J";r$l=`tput lines`-2,s/^$/"\e[$=;".r($c
=`tput cols`-2).f.' Oo*... '=~s!\S\K!\e[B\b !gr/e/s/\d++\B/($a=$&-1)+($
==!$a+r$a%$c?3:2)/e/s/(?<=(.)...B.)./$1/g/s/f\K./chr 32>>$=/e/s/\d+/($&
||$l)-1/e,select$,,$,,$,,.009*print$&?$_:"\e[f"."\e[K\n"x6while*_=r.r+9
