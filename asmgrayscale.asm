; S19B Heisel Janine Lazaro
; S19B Justine Erika Wee
; Program: Mapping Single Precision Float to uint8

section .data
pixel_val dd 255.0  ; constant pixel scale value

section .text
default rel
bits 64
global imgCvtGrayFloatToInt

imgCvtGrayFloatToInt:
    MOV rax, rcx            ; rax = height
    IMUL rax, rdx           ; rdx = width, rax = total number of pixels
    
    CMP rax, 0
    JE L5_end               ; if no pixels, end

    MOVSS xmm1, dword [pixel_val] ; load 255.0f

L1_start:
    CMP rax, 0
    JE L5_end               ; check if loop is done

    ; calculation and conversion
    MOVSS xmm0, [r8]        ; load float input        
    MULSS xmm0, xmm1        ; multiply pixel by 255
    CVTTSS2SI ebx, xmm0     ; convert float to int

    ; clamp values to stay in range
    CMP ebx, 0
    JL L3_zero              ; if < 0, set to 0
    CMP ebx, 255
    JG L4_255               ; if > 255, set to 255

L2_store:                   
    MOV byte [r9], bl       ; store integer output
    ADD r8, 4                
    ADD r9, 1                
    DEC rax                  
    JMP L1_start            ; process next pixel

L3_zero:                    
    XOR ebx, ebx             
    JMP L2_store

L4_255:
    MOV ebx, 255             
    JMP L2_store

L5_end:
    ret                      