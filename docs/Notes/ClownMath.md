## Keeping track of my own circus math

This document is not meant to present any kind of novel, groundbreaking math. 
It is not even _good_ math. It is simply here so I can refer to it in the future, should I ever be brave enough to lay eyes 'pon my own source code.  
If you like math, or if you're even on friendly terms with math, I'd suggest you look away from the crimes I am about to commit in this here document.

### Contents
+ [The worst, Frankensteinian RGB algorithm](#rgbtuplegeneration)
+ [RGB tuple to full saturate by taking the scenic route](#rgbtohue)
---

### Generating fully saturated RGB tuples in a for-loop by making modular math weep<a name="rgbtuplegeneration"></a>  
Given the colour tuple $(R, G, B)$, it is possible to generate a set of all fully saturated RGB tuples, meaning tuples where one value is set to $0$, one value is set to $225$ and one value is either ascending or descending,  through a for-loop in such a way that the following pattern emerges:  
>$(255, \uparrow, 0), (\downarrow, 255, 0), (0, 255, \uparrow), (0, \downarrow, 255), (\uparrow, 0, 255),  (255, 0, \downarrow)$   

The pattern is cyclical and can be divided into $6$ equal subsets of length $225$, therefore the total number of possible RGB tuples is $1530$.
 
For indexing purposes (and because I've decided to set it up this way in the code for reasons that currently elude me), the initial condition of this for-loop is $(B, G, R) = (0, 0, 255)$, which is essentially just the normal tuple for a fully saturated red, but in reverse.  
The for-loop itself is structured as follows:  

```c
InitialBGR = {0, 0, 255}  //Fully saturated red in reverse

for(int TupleCombination in int AllPossibleCombinations)
{  
  for(int ChannelIndex in length(InitialBGR)) 
  {
    InitialBGR[ChannelIndex] += //Weird Wizard Math(tm)
  }
}
```
Think of it as gradually adjusting `InitialBGR` for each iteration in the 1530 total possible iterations; adjusting B first, then G and lastly R.

#### 1. Keeping track of what subset we're working through  
Since we are essentially working through six different subsets, we can keep track of which subset we're currently working in through the magic of floor division. You can think of this as keeping track of what 'region' in the total 1530 combinations we're in at the current moment.
```c
Subset = int(TupleCombination / 255) + 1
```
This ensures that we can mathematically refer to each subset with numbers 1 through 6. Not 0 through 5, because we want to _count_ subsets. Having a 0 show up here might break the math down the line so it is better to avoid that altogether. The subsets are as follows:  

|Subset    | Tuple configuration    |
|:---------|:-----------------------|
|    1     | $(255, \uparrow, 0)$   |
|    2     | $(\downarrow, 255, 0)$ |
|    3     | $(0, 255, \uparrow)$   |
|    4     | $(0, \downarrow, 255)$ |
|    5     | $(\uparrow, 0, 255)$   |
|    6     | $(255, 0, \downarrow)$ |

#### 2. What are we even trying to do here?  
The idea is to encode three patterns into this 1530 step for-loop. For each subset, which is really just a chunk in the for-loop of size 255, apply an addition to continuously modified `InitialBGR` for each channel in $(B, G, R)$. The addition can either be $1$, $0$ or $-1$, which will allow each colour channel to rise and fall between $0$ and $255$.  
The addition patterns, listed per subset, are as follows:
>$R_+:\{0, -1, 0, 0, 1, 0\}$  
>$G_+:\{1, 0, 0, -1, 0, 0\}$  
>$B_+:\{0, 0, 1, 0, 0, -1\}$

So for subset 1 (read: `AllPossibleCombinations` 0 through 254) we only increment Green, then in subset 2 we only decrement Red, and so on.

#### 3. Isolating the subsets where Blue is locked  
Looking at the subsets it appears the Blue channel is locked in subsets 1, 2, 4 and 5. By multiplying these numbers we get a number whose `mod Subset` is only non-zero for subsets 3 and 6.  
>$(1 * 2 * 4 * 5) = 40$  
  
```c
//So far we have
InitialBGR[Channel] += 40 % Subset
```

This will increment all channels in subsets 3 or 6, by `40 mod [3, 6]` or $\{1, 4\}$ respectively. Meaning the addition pattern for each channel becomes:  
>$R_+ = G_+ = B_+:\{0, 0, 1, 0, 0, 4\}$

#### 4. Roping Green and Red into the equation  
The Green channel only changes in subsets 1 and 4, and here we can take advantage of `ChannelIndex`. 
By adding $(ChannelIndex * 2)$ to the modulus, we change how the modulation works for the Red and Green channels. The Blue channel remains unchanged as its ChannelIndex is always 0, thus it does not factor into the modulus.
```c
//Modifying modulus
InitialBGR[ChannelIndex] += 40 % (Subset + (ChannelIndex * 2))
```
For the Green channel this means that the modulus for each subset becomes  
>$G_m:\{1, 2, 3, 4, 5, 6\} + (1 * 2) \rightarrow \{3, 4, 5, 6, 7, 8\}$  

Where 3, 6 and 7 are the only moduli which are **not** a factor of 40, meaning that for the subsets 1, 4 and 5 the Green channel will be incremented by `40 mod [3, 6, 7]` or $\{1, 4, 5\}$ respectively. Meaning the addition pattern becomes:  
>$G_+:\{1, 0, 0, 4, 5, 0\}$  

For the Red channel this means that the modulus for each subset becomes  
>$R_m:\{1, 2, 3, 4, 5, 6\} + (2 * 2) \rightarrow \{5, 6, 7, 8, 9, 10\}$  

Where 6, 7 and 9 are the only moduli which are **not** a factor of 40, meaning that for the subsets 2, 3 and 5 the Red channel will be incremented by `40 mod [6, 7, 9]` or $\{4, 5, 4\}$ respectively. Meaning the addition pattern becomes:  
>$R_+:\{0, 4, 5, 0, 4, 0\}$  

Because of this change, Green no longer increments in subset 3 and 6 because `40 mod [5, 8] = 0`. Red no longer increments in subset 6 since `40 mod 10 = 0`.  

Because the `ChannelIndex` of Blue is 0, modifying the modulus has no effect so the addition pattern of Blue remains:  
>$B_+:\{0, 0, 1, 0, 0, 4\}$  

#### 5. Curbing growth for all channels
Currently, all channels ever only increment and mainly do so with values larger than 1; namely 4 or 5. We can once again make use of floor division to isolate the subsets where these larger additions happen.
```c
int((40 % (Subset + (ChannelIndex * 2))) / 4)
```

Which means:
>$\ $   
>${\Large\lfloor\frac{R_+:\{0, 4, 5, 0, 4, 0\}}{4} \rfloor} \rightarrow R_f:\{0,1,1,0,1,0\}$   
>$\ $   
>${\Large\lfloor\frac{G_+:\{1, 0, 0, 4, 5, 0\}}{4} \rfloor} \rightarrow G_f:\{0,0,0,1,1,0\}$  
>$\ $   
>${\Large\lfloor\frac{B_+:\{0, 0, 1, 0, 0, 4\}}{4} \rfloor} \rightarrow B_f:\{0,0,0,0,0,1\}$  
>$\ $   


Now, by multiplying each value in $RGB_f$ by $-5$ and adding $RGB_f$ to $RGB_+$, we can change the additions as such: 

>$R_+:\{0, 4, 5, 0, 4, 0\} + R_f:\{0,-5,-5,0,-5,0\} \rightarrow \{0, -1, 0, 0, -1, 0\}$  
>$G_+:\{1, 0, 0, 4, 5, 0\} + G_f:\{0,0,0,-5,-5,0\} \ \ \: \rightarrow \{1, 0, 0, -1, 0, 0\}$  
>$B_+:\{0, 0, 1, 0, 0, 4\} + B_f:\{0,0,0,0,0,-5\} \quad \ \; \rightarrow \{0, 0, 1, 0, 0, -1\}$

This, then, fixes Green, Blue and _most of_ Red, which now has a subtraction in subset 5 which ought to be an addition.  
Thus far, the formula as a whole is:
```c
//Curbing growth
InitialBGR[ChannelIndex] += 40 % (Subset + (ChannelIndex * 2))
                     - (5 * (int((40 % (Subset + (ChannelIndex * 2))) / 4)))
```
Which is already starting to look incredibly stupid, but at least correctly calculates subsets 1 through 4.

#### 6. Fixing Red's subtraction in subset 5 with more modular mayhem 
The last thing to tweak is Red's addition pattern, which currently is:  
>$R_+:\{0, -1, 0, 0, -1, 0\}$  

So we need to find a way to add $2$ to this addition pattern in subset 5. Which can be done, once again, using floor division with the following tidbit:
```c
2 * (int((Subset + (ChannelIndex * 2)) / 9))
```
The floor division will only evaluate to $1$ while working with subset 5 or 6, meaning it will add $2$ to to these subsets in $R_+$.  However, this needs to trigger _if and only if_ the subset is 5. This is where the final piece slots in:
```c
10 % (Subset + (ChannelIndex * 2))
```
This will evaluate to $1$ when the subset is 5 and $0$ when the subset is 6.  
It will also evalute to non-zero numbers in subsets 1 through 4, but since we multiply this by the result of the floor division by 9 tidbit, these positive values for subsets 1 through 4 will be multiplied by $0$ and thus be rendered moot.   
Putting it altogether yields:
```c
InitialBGR = {0, 0, 255}

for(int TupleCombination in int AllPossibleCombinations)
{  
  for(int ChannelIndex in length(InitialBGR))
  {    
                             //Lock Blue channel, modify moduli for Red/Green
    InitialBGR[ChannelIndex] += (40 % (Subset + (ChannelIndex * 2))) 
                             //Curbing growth and setting negatives
                             - (5 * (int((40 % (Subset + (ChannelIndex * 2))) / 4))) 
                             //Fix Red addition by adding 2, but only in subset (5, 6) AND (2, 3, 4, 5)
                             //resolving to only in subset 5 
                             + (2 * (int((Subset + (ChannelIndex * 2)) / 9)) * (10 % (Subset + (ChannelIndex * 2))))
  }
  print(InitialGBR[2], InitialBGR[1], InitialBGR[0])   // Fully saturated (R, G, B) combination!
}
```

And there you have it, the silliest algorithm to generate a full set of all possible fully saturated RGB tuples. You could likely collapse this down to just one for-loop by iterating over `AllPossibleCombinations` thrice and then using some more modular math wizardry to determine whether to modify $R$, $G$ or $B$, but I do believe this unwarranted attack on math has gone on for long enough, so here is where I'll draw the line.  
_for now._

---

### Turning an RGB tuple into its fully saturated colour <a name = "rgbtohue"></a>

Given any colour tuple $(R, G, B)$, you can derive its fully saturated tuple by setting the lowest value  $l$ to 0, the highest value $h$ to 255 and calculating the middle value $m$ to $m_c$.  
In essence, an example tuple like $(212, 44, 81)$, will turn into $(255, 0, m_c)$.  

#### 1. Scale all values towards 255, relatively  
The first step is to 'pull up' $h$ to 255 with $m$ and $l$ scaling relatively alongside. You can think of this as scaling the Value to 100% in the HSV model. Not all values will scale evenly, but relative to the difference between $h$ and 255.

$\Delta_h = 255 - h$

Which then scales values $m$ and $l$ as such:  

$u' = \dfrac{u}{h}\Delta_h + u$  
Where $u$ is either $m$ or $l$.

#### 2. Scale $m'$ and $l'$ values down towards 0, relatively
This is analogous to scaling the Saturation to 100% in the HSV model.  
Because I am an idiot, I prefer to think of this not so much as 'pulling down' the values, but rather 'pulling up' its remainder. So instead of bringing $l'$ down to 0, I'd like to pull $(255 - l')$ up to 255. Now I just need the amount of $m'$ scaled relative to $l'$. This will be, however, the _inverse_ of the actual scaling, because I am doing all this remainder wizardry. Therefore, I need to subtract the value from 255.  
Is this a good way of doing it? Absolutely not, but it is how I've decided to tackle this problem so I guess I am stuck with it. 

$m_c = 255 - \biggl(\dfrac{225 - m'}{255 -l'}255\biggr)$ 

Which looks stupid, so we can boil it down to:  
(And yes, this is verbose as all hell, but I am trying to get my squirrel brain to follow along since I am bad at math.)  

1. Resolve multiplying the fraction by 255    
$\ $   
$m_c = 255 - \biggl(\dfrac{225^2 - 255m'}{255 -l'}\biggr)$   
 
2. Distribute 255 into the fraction  
$\ $  
$m_c =\dfrac{255(255 -l') - (225^2 - 255m')}{255 -l'}$   

3. Resolve the multiplication of 255   
$\ $  
$m_c =\dfrac{(255^2 -255l') - (225^2 - 255m')}{255 -l'}$  

4. Cancel out the squares, mind the double negative!  
$\ $  
$m_c =\dfrac{-255l' - (-255m')}{255 -l'}$  

5. Resolve the double negative  
$\ $  
$m_c =\dfrac{-255l'  + 255m'}{255 -l'}$  

6. Swapsies for readability  
$\ $  
$m_c =\dfrac{255m' - 255l'}{255 -l'}$  

7. Breaking out common 255  
$\ $  
$m_c =\dfrac{255(m' - l')}{255 -l'}$  

This, eventually, yields $m_c$ in the earlier given $(255, 0, m_C)$.  
And that would be enough, a simple 2-step process. But because I am a doofus, I realise I can just fully substitute $m'$ and $l'$ into this equation. And after subbing in $x$ for 255 it gives rise to the behemoth that is:

$m_c = \dfrac{x\biggl(\biggl(\dfrac{m}{h}(x-h)+m\biggr) - \biggl(\dfrac{l}{h}(x-h)+l\biggr)\biggr)}{x-\biggl(\dfrac{l}{h}(x-h)+l\biggr)}$

Look at it.  
It's massive.  
It looks stupid. You could probably beat someone to death with it if you printed it all out on a baseball bat. This simply will not do. 

#### 3. I don't like implementing simple two-step solutions so I will make things harder for myself 

_Subtitle:  I am bad at math so this will look stupid._  

Let's first focus on the construct $\biggl(\dfrac{u}{h}(x-h)+u\biggr)$, since it appears thrice.  

1. Resolve the multiplication  
$\ $  
$\dfrac{u(x-h)}{h}+u$

2. Distribute $u$ into the fraction  
$\ $  
$\dfrac{u(x-h) + uh}{h}$

3. Resolve multiplication  
$\ $  
$\dfrac{(ux-uh) + uh}{h}$

4. Cancel out $uh$  
$\ $  
$\dfrac{ux}{h}$

Great, plugging that back into the main body nets us:

$m_c = \dfrac{x\biggl(\dfrac{mx}{h} - \dfrac{lx}{h}\biggr)}{x-\dfrac{lx}{h}}$

This looks a whole lot better than the be-not-afraid-esque page filler I started out with, but it can be simplified down even more.

1. Multiply $x$ into the fraction in the numerator  
$\ $  
$m_c = \dfrac{\dfrac{mx^2 - lx^2}{h}}{x-\dfrac{lx}{h}}$

2. Distribute $x$ into the fraction in the denominator  
$\ $  
$m_c = \dfrac{\dfrac{mx^2 - lx^2}{h}}{\dfrac{hx- lx}{h}}$

3. Resolve the major fraction  
$\ $  
$m_c = \dfrac{mx^2 - lx^2}{h}   \dfrac{h}{hx- lx}$  

4. Simplify further  
$\ $  
$m_c = \dfrac{h(mx^2 - lx^2)}{h(hx-lx)}$

5. Divide by $h$  
$\ $  
$m_c = \dfrac{mx^2 - lx^2}{hx-lx}$

6. Divide by $x$  
$\ $  
$m_c = \dfrac{mx - lx}{h-l}$

7. Break out  $x$  
$\ $  
$m_c = \dfrac{x(m - l)}{h-l}$

Excellent, so after all that faffing about (no doubt doubling back a few times and doing this in the least efficient way possible) we have arrived at the way to determine $m_c$ in  $(255, 0, m_c)$ when given an arbitrary RGB tuple like $(212, 44, 81)$.


$m_c = \dfrac{255(81 - 44)}{212-44} \approx 56$ 

#### 4. Special cases, because *of course* there always have to be exceptions

The above formula works for RGB tuples where each colour channel is unique, but in tuples with duplicates (or triplicates?) the process is actually a lot quicker and doesn't involve any weird squirrel math.

- **Option 1 - There's a duplicate value: $(A, A, B)$**  
  In this case it depends on which value is larger
  - A > B : Set A to 255, B to 0 $(255, 255, 0)$   
  - A < B : Set A to 0, B to 255 $(0, 0, 255)$

- **Option 2 - Why use colours when it could just be pure grey: $(A, A, A)$**  
  In this case it makes no sense to calculate any value, since you can neither find a maximum, nor a minimum value. Even if you could you'd end up dividing by zero which I recall my high school math teacher would get very worked up about. In this case it makes more sense to just return (255, 255, 255) and call it a day.


