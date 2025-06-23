//플레이어가 SafeZone(안전 구역)을 벗어났는지 체크하는 스크립트
using System.Globalization;
using UnityEngine;
using Unity.Netcode;

public class PlayerOutZoneDetector : NetworkBehaviour
{
    private Collider zoneCollider = null;   //SafeZone의 콜라이더
    private float fCheckInterval = 1.0f;    //체크 간격 (초)
    private float fTimer = 0.0f;            //체크를 위한 타이머

    void Start()
    {
        GameObject zoneObject = GameObject.FindGameObjectWithTag("SafeZone");

        if (zoneObject != null)
        {
            zoneCollider = zoneObject.GetComponent<Collider>();
        }

        if (zoneCollider == null)
        {
            Debug.LogError("SafeZone 콜라이더가 없습니다. 태그가 'SafeZone'이고 콜라이더 컴포넌트가 있는지 확인하세요.");
        }
    }

    void Update()
    {
        if (!IsOwner) return;
        if (zoneCollider == null) return; //SafeZone 콜라이더가 없으면 체크하지 않음

        fTimer += Time.deltaTime;

        if (fTimer >= fCheckInterval) //1초 마다 체크, 시스템 자원 절약
        {
            fTimer = 0.0f;
            
            f_CheckOutOfZone(); //SafeZone 밖에 있는지 검사 후 처리
        }
    }

    /// <summary> SafeZone 밖에 있는지 검사 후 처리하는 메소드 </summary>
    void f_CheckOutOfZone()
    {
        Vector3 pos = transform.position;

        if (!zoneCollider.bounds.Contains(pos))
        {
            Debug.Log("플레이어가 안전 구역 밖에 있음");

            PlayerHealthNet health = GetComponent<PlayerHealthNet>();
            
            if (health != null)
            {
                health.health.Value = 0;

                SoundManager.Instance.f_PlaySFX(SoundName.SFX_GameOver, 0.7f); //게임오버 효과음 재생

                Invoke("f_GameOver", 2.0f); //2초 후 게임오버 처리
            }
        }
    }

    /// <summary> nvoke를 통해 호출되는 게임오버 처리 메소드 </summary>
    void f_GameOver()
    {
        FlowManager.Instance.f_OpenScene(SceneName.EndScene); //게임오버 씬으로 전환
    }
}