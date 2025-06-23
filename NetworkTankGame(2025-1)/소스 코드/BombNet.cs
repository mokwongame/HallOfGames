using UnityEngine;
using Unity.Netcode;

public class BombNet : NetworkBehaviour
{
    Rigidbody rbBomb; //BombNet 스크립트가 부착된 오브젝트의 Rigidbody 컴포넌트
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        if (!IsOwner) return; //IsOwner가 false면 이 BombNet 오브젝트는 소유자가 아니므로 초기화하지 않음

        rbBomb = GetComponent<Rigidbody>();
       
        shotBombRpc(); //포탄 발사 효과음 재생 및 포탄에 힘을 가하는 RPC 호출

        Invoke("killBombRpc", 4.0f); //4초 뒤에 killBombRpc() 호출해서 포탄을 파괴
    }

    // Update is called once per frame
    void Update()
    {

    }

    [Rpc(SendTo.Owner)]
    void shotBombRpc()
    {
        SoundManager.Instance.f_PlaySFX(SoundName.SFX_CannonFire, 0.7f); //포탄 발사 효과음 재생
        
        rbBomb.AddForce(transform.up * GameManager.Instance.ForceBomb);
    }

    [Rpc(SendTo.Server)] //Despawn()은 server RPC만 가능
    void killBombRpc()
    {
        NetworkObject.Despawn(); //Despawn()은 Spawn()의 반대말: 영어의 de는 없앤다는 뜻
    }

    private void OnCollisionEnter(Collision collision)
    {
        //현재 위치는 BombRed(tag = Bomb)
        string tag = collision.gameObject.tag;  //충돌한 대상체의 tag
        if (tag == "Plane")                     //땅에 부딪힘
        {
            makeExplosionRpc(); //폭발 효과 생성
            killBombRpc();      //포탄 파괴
        }
        else if (tag == "Player") //상대방 탱크에 충돌
        {
            PlayerHealthNet player = collision.gameObject.GetComponent<PlayerHealthNet>(); //충돌한 탱크의 PlayerHealthNet 컴포넌트 가져오기
            if (player != null)
            {
                player.decHealthRpc(); //충돌한 탱크의 체력 감소 RPC 호출

                /* [기능 추가]
                 * 반작용 힘 적용 본인이 자기 Rigidbody에 Force 가하도록 Owner RPC 호출
                 * (네트워크 게임 특성 적용 ※PlayerHealthNet에 자세한 주석)
                 */
                Vector3 direction = (collision.transform.position - transform.position).normalized;
                player.f_ApplyPushForceRpc(direction);
            }

            //폭발 효과 및 포탄 파괴
            makeExplosionRpc();
            killBombRpc();
        }
    }

    [Rpc(SendTo.Server)]
    void makeExplosionRpc()
    {
        GameObject prefabFile = Resources.Load("ExplosionRed") as GameObject;
        GameObject prefab = Instantiate(prefabFile);
        
        prefab.transform.position = transform.position;
        prefab.transform.rotation = transform.rotation;
        prefab.GetComponent<NetworkObject>().Spawn(); //server RPC만 Spawn() 메소드 호출 가능

        SoundManager.Instance.f_PlaySFX(SoundName.SFX_Explosive, 0.7f); //폭발 효과음 재생
    }
}
