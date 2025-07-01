# Nextion Example Usage

Bu klasör Nextion kütüphanesi için örnek kullanım kodlarını içerir.

## Dosyalar

### `u_nextion_example.c`
Nextion kütüphanesinin temel kullanımını gösteren örnek kod.

## Kullanım

Bu örnek kodu kullanmak için:

1. `main.c` dosyanızı bu örnekle değiştirin
2. Projenizi derleyin ve yükleyin

## Özellikler

- **Switch-Case Yapısı**: Otomatik düzeltme tarafından değiştirilmeyen switch-case formatı
- **Action Type Processing**: Nextion'dan gelen verileri action type'a göre işleme
- **Task Management**: Nextion data mapping task'ı ile asenkron veri işleme

## Kod Yapısı

```c
typedef enum
{
    ACTION_TYPE_DENEME,
} action_type_t;

void nextion_data_mapping_task(void *params)
{
    while (1)
    {
        action_type_t action_type = (action_type_t)nextion_data.action_type;
        switch (action_type)
        {
        case ACTION_TYPE_DENEME:
        {
            printf("Action Type: %d, Action Data: %s\n", action_type, nextion_data.action_data);
        }
        break;
        default:
        {
            printf("data not found");
        }
        break;
        }
        vTaskSuspend(NULL);
    }
}
```

## Nextion Veri Formatı

Nextion'dan gelen veriler şu formatta olmalıdır:
```
~[0]deneme_verisi
```

Burada:
- `~[` : Başlangıç işareti
- `0` : Action type (ACTION_TYPE_DENEME = 0)
- `]` : Action type sonu
- `deneme_verisi` : İşlenecek veri

## Konfigürasyon

Menuconfig'de Nextion ayarlarını yapılandırın:
- UART Port: 2
- TX Pin: 43
- RX Pin: 44
- Baud Rate: 115200 