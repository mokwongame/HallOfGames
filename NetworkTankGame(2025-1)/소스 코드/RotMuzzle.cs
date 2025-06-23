using UnityEngine;
using Unity.Netcode;

public class RotMuzzle : NetworkBehaviour
{
    float angMax = 0.0f;
    float angMin = -15.0f;

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (!IsOwner) return;

        int keytype = 0;
        if (Input.GetKey(KeyCode.I)) // 모든 프레임마다 키 입력 감지
        {
            keytype = 1;
        }
        else if (Input.GetKey(KeyCode.K))
        { keytype = -1; }

        //Quaternion rot = transform.rotation; // 어려움
        // 쉬운 오일러 각을 사용
        Vector3 rot = transform.localEulerAngles; // Unity Editor(-180~180도)에 나오는 각도: 0~360도
        double xang = rot.x;
        float angoff = -keytype * GameManager.Instance.RotSpeedTank * Time.deltaTime;
        xang += angoff;
        // Unity Editor의 각도와 localEulerAngles의 각도를 (-180~180도)로 통일
        if (xang > 180) xang -= 360;
        //Debug.Log($"xang = {xang}");
        if (xang >= angMin && xang <= angMax)
            transform.Rotate(angoff, 0.0f, 0.0f);

        // 포탄(bomb) 발사
        if (Input.GetKeyDown(KeyCode.Space))
        {
            makeBombRpc();
        }
    }

    [Rpc(SendTo.Server)]
    void makeBombRpc()
    {
        GameObject prefabFile = Resources.Load("BombRed") as GameObject;
        // prefab의 예시 만들기(instantiate)
        GameObject prefab = Instantiate(prefabFile);
        prefab.transform.position = transform.position + transform.forward * 5.7f + transform.up * 0.0f; // transform은 플레이어의 변환
        prefab.transform.rotation = transform.rotation;
        prefab.transform.Rotate(90.0f, 0.0f, 0.0f); // 포탄을 x축을 회전축으로 90도 돌림
        prefab.GetComponent<NetworkObject>().Spawn(); // server RPC만 Spawn() 메소드 호출 가능
    }
}
