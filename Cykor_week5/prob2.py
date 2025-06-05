Flag_value = "TPBFI{N3CTFD3_2_TIPGKFXI4GP!}"
String = list(Flag_value)

for j in range(len(String)):
    for i in range(len(String)):
        if String[i] >= 'A' and String[i] <= 'Z':
            String[i] = chr(ord(String[i]) + 1)
            if String[i] > 'Z':
                String[i] = 'A'
    for k in range(len(String)):
        print(String[k], end='')
    print()

# for i in range(len(String)):
#     print(String[i], end='')
