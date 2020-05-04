# Donkey Kong Jr

This is an adaptation for the [Gamebuino META][gamebuino] of the famous eponymous game, released in 1982 by Nintendo on his Game & Watch system.

I tried to stay consistent with the original version by scrupulously examining some demonstration videos on YouTube, but this task proved to be laborious and imprecise... I think I have managed to make a fairly faithful adaptation despite everything. You'll tell me what you think :-)

<table cols="2">
    <tbody>
        <tr>
            <td>
                <img width="428" src="https://gamebuino.m1cr0lab.com/share/donkey-kong-jr/game-and-watch-1380x828.jpg" alt="Original Game & Watch" />
            </td>
            <td>
                <img width="428" src="https://gamebuino.m1cr0lab.com/share/donkey-kong-jr/demo-856x462.gif" alt="Demo" />
            </td>
        </tr>
    </tbody>
</table>

This project was born out of [a request from Jicehel][request] in the beginning, who wanted a programming model on which to base itself to make another game, also released on Nintendo Game & Watch system: [Parachute][parachute]. I then provided him with a demo code implementing a methodology for making this type of game with the Gamebuino META. And I chose to apply it to a draft of Donkey Kong Jr. After a few days, I finally decided to make the game in its entirety.

The game uses the full resolution of the META: 160x128 pixels with a 16-bit color depth. Graphics rendering is done with low-level techniques based on the `gb.tft` API, and does not use the standard `gb.display` API.

## The story of the game

Mario has locked Donkey Kong in a cage, and you play the role of his son, Junior, who is trying by all means to save him. To open the cage, you will have to collect successively the four necessary keys, while avoiding the dangers that will arise in your path.

## Instructions for use

<img width="600" src="https://gamebuino.m1cr0lab.com/share/donkey-kong-jr/gamebuino-meta-1399x551.png" alt="How to play" />

Button A starts Game A.  
Button B starts Game B.  
The pace of Game B is much faster than that of Game A.

Move Junior using the D-PAD, and press the A button to jump.  
But beware, the pace of the game is accelerating over time...

Use button B to turn off the sound effects.

You can also use the MENU button to alternate the display of the score, CPU load and available memory space. I had added this feature for debugging, and finally kept it. It may help you to see that the optimization of the graphic rendering is quite successful  😎

## Source code

The source code is fully documented to help you better understand the implementation and create your own adaptations of Game & Watch on Gamebuino META.

## Acknowledgements

I would like to thank [Andy][aoneill], who explained us how to use the DMA controller to optimize framebuffer transfers to the display device and, at the same time, exploit the full resolution of the META (160x128 pixels in 16-bit color depth).

I would also like to thank [Valden][valden] who released [a small utility][gsfxgui] very useful to easily set up and integrate procedural sounds into our Gamebuino applications. I had initially chosen to integrate sound effects in the WAV format, but reading this format at runtime consumes far too much CPU, and I finally gave up the idea.

[gamebuino]: https://gamebuino.com/
[request]:   https://community.gamebuino.com/t/demande-de-squelette-resolu/943
[parachute]: https://community.gamebuino.com/t/parachute-hd-meta/964
[aoneill]:   https://gamebuino.com/@aoneill
[valden]:    https://gamebuino.com/@valden
[gsfxgui]:   https://gamebuino.com/creations/gamebuino-sound-fx-creation-tool-gsfx-gui