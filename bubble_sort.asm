; IITPSx Bubble Sort - 5 element standard demonstration
; Input: 5, 1, 4, 2, 8
; Expected output: 1, 2, 4, 5, 8
; Adjacent comparisons are unrolled for clarity.

; compare array[0] and array[1]
        ldc array
        ldnl 0
        ldc array
        ldnl 1
        sub
        brlz next00
        brz next00
        ; save left value
        ldc array
        ldnl 0
        ldc temp
        stnl 0
        ; right -> left
        ldc array
        ldnl 1
        ldc array
        stnl 0
        ; saved left -> right
        ldc temp
        ldnl 0
        ldc array
        stnl 1
next00:
; compare array[1] and array[2]
        ldc array
        ldnl 1
        ldc array
        ldnl 2
        sub
        brlz next01
        brz next01
        ; save left value
        ldc array
        ldnl 1
        ldc temp
        stnl 0
        ; right -> left
        ldc array
        ldnl 2
        ldc array
        stnl 1
        ; saved left -> right
        ldc temp
        ldnl 0
        ldc array
        stnl 2
next01:
; compare array[2] and array[3]
        ldc array
        ldnl 2
        ldc array
        ldnl 3
        sub
        brlz next02
        brz next02
        ; save left value
        ldc array
        ldnl 2
        ldc temp
        stnl 0
        ; right -> left
        ldc array
        ldnl 3
        ldc array
        stnl 2
        ; saved left -> right
        ldc temp
        ldnl 0
        ldc array
        stnl 3
next02:
; compare array[3] and array[4]
        ldc array
        ldnl 3
        ldc array
        ldnl 4
        sub
        brlz next03
        brz next03
        ; save left value
        ldc array
        ldnl 3
        ldc temp
        stnl 0
        ; right -> left
        ldc array
        ldnl 4
        ldc array
        stnl 3
        ; saved left -> right
        ldc temp
        ldnl 0
        ldc array
        stnl 4
next03:
; compare array[0] and array[1]
        ldc array
        ldnl 0
        ldc array
        ldnl 1
        sub
        brlz next04
        brz next04
        ; save left value
        ldc array
        ldnl 0
        ldc temp
        stnl 0
        ; right -> left
        ldc array
        ldnl 1
        ldc array
        stnl 0
        ; saved left -> right
        ldc temp
        ldnl 0
        ldc array
        stnl 1
next04:
; compare array[1] and array[2]
        ldc array
        ldnl 1
        ldc array
        ldnl 2
        sub
        brlz next05
        brz next05
        ; save left value
        ldc array
        ldnl 1
        ldc temp
        stnl 0
        ; right -> left
        ldc array
        ldnl 2
        ldc array
        stnl 1
        ; saved left -> right
        ldc temp
        ldnl 0
        ldc array
        stnl 2
next05:
; compare array[2] and array[3]
        ldc array
        ldnl 2
        ldc array
        ldnl 3
        sub
        brlz next06
        brz next06
        ; save left value
        ldc array
        ldnl 2
        ldc temp
        stnl 0
        ; right -> left
        ldc array
        ldnl 3
        ldc array
        stnl 2
        ; saved left -> right
        ldc temp
        ldnl 0
        ldc array
        stnl 3
next06:
; compare array[0] and array[1]
        ldc array
        ldnl 0
        ldc array
        ldnl 1
        sub
        brlz next07
        brz next07
        ; save left value
        ldc array
        ldnl 0
        ldc temp
        stnl 0
        ; right -> left
        ldc array
        ldnl 1
        ldc array
        stnl 0
        ; saved left -> right
        ldc temp
        ldnl 0
        ldc array
        stnl 1
next07:
; compare array[1] and array[2]
        ldc array
        ldnl 1
        ldc array
        ldnl 2
        sub
        brlz next08
        brz next08
        ; save left value
        ldc array
        ldnl 1
        ldc temp
        stnl 0
        ; right -> left
        ldc array
        ldnl 2
        ldc array
        stnl 1
        ; saved left -> right
        ldc temp
        ldnl 0
        ldc array
        stnl 2
next08:
; compare array[0] and array[1]
        ldc array
        ldnl 0
        ldc array
        ldnl 1
        sub
        brlz next09
        brz next09
        ; save left value
        ldc array
        ldnl 0
        ldc temp
        stnl 0
        ; right -> left
        ldc array
        ldnl 1
        ldc array
        stnl 0
        ; saved left -> right
        ldc temp
        ldnl 0
        ldc array
        stnl 1
next09:
done:   HALT

temp:   data 0
array:  data 5
data 1
data 4
data 2
data 8
