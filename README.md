Introduction
================
Nowadays most of the mobile devices include embeded blue-tooth capability, This article presents an idea about using these blue-tooth devices, even lower end ones for causal remote control. IrDA’s  OBEX protocol is primarily a proprietary protocol, the specification are only accessible from IrDA by paying a fees. By choosing OBEX protocol, we limit our options, and we invite the hassle to implement  little publicly known OBEX protocol ourself. But by using OBEX,  we can support even lower end mobile device, since many supports OBEX file push profile.

Most of the blue-tooth devices (i.e mobiles) including lower end devices, supports OBEX (OBEX file push), this prompts the paired device to show a dialog, with notice to accept or reject the file (OBEX object),  these prompts usually have some timeout associated with them. Therefore we are only left with OBEX accept as the only definitive option. This doesn’t look very useful, but you can use a single option as a remote control for controlling presentation slides. This should vastly improve the presenter’s and audience’s experience, on average this could provide seamless wireless ppt control over 10-20 meters or more.

PPTMote
==================
PPTMote (PPT reMote) is a little program that uses blue-tooth OBEX file prompt’s responses to stimulate virtual key presses as required.

To provide seamless experience without any lags “pptmote” is entriely written in native code, in C, uses Microsoft’s native Bluetooth stack. Since Microsoft’s Bluetooth stack doesnt provide any implicit support for OBEX, pptmote implements the required OBEX support in hard-coded OBEX headers.

Bluetooth stack is more or less like a TCP/IP networking stack, this entire idea is implmented over rfcomm sockets, which provides TCP like capability. (i.e QoS)

For this purpose, I intercepted the bluetooth communication to determine the OBEX protocol, the result are as follows:

On successful communication, the client send a OBEX connect packet, which is as follows

OBEX CONNECT
===================

0000-0010:  80 00 1a 10-00 ff ff 46-00 13 f9 ec-7b c4 95 3c  .......F ....{..<
0000-001a:  11 d2 98 4e-52 54 00 dc-9e 09                    ...NRT.. ..

80    : Connect
00 1A : length of package, here 26 bytes
10    : obex version 1.0
00    : always zero
ff ff : maximum package length
46    : Header ID
00 13 : Total Header length 0x13
After that, the OBEX file prompt is sent,

OBEX PUT
===================

0000-0010:  02 00 1b 01-00 13 00 70-00 70 00 74-00 6d 00 6f  .......p .p.t.m.o
0000-001b:  00 74 00 65-00 00 c3 00-00 00 07                 .t.e.... ...

If the user accepts, the required keys are stimulated and At last the OBEX disconnect packet is sent, before closing the rfcomm socket connections.

OBEX DISCONNECT
========================

0000-0003:  81 00 03                                         ...

The program automatically finds the OBEX Object Push using UUID and SDP (service discovery protocol)

How to run
==================

Make sure that your mobile display a prompt when a file pushed to it

Pair your device with your PC

Start the program, enter the device name, default is pptmote

Specify the key to emulate, default is SPACE key

Click on "CONNECT"

That’s it!, file prompts will be shown on your device, accepting will emulate your selected key on your PC

Close the program, when you are done.
