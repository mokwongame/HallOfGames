using UnityEngine;
using Unity.Netcode;

public class RotTurret : NetworkBehaviour
{
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        int keytype = 0;
        if (Input.GetKey(KeyCode.J)) // 모든 프레임마다 키 입력 감지
        {
            keytype = -1;
        }
        else if (Input.GetKey(KeyCode.L))
        { keytype = 1; }

        float angoff = keytype * GameManager.Instance.RotSpeedTank * Time.deltaTime;
        transform.Rotate(0.0f, angoff, 0.0f);
    }
}
