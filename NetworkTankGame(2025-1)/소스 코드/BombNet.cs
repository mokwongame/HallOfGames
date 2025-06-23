using UnityEngine;
using Unity.Netcode;

public class BombNet : NetworkBehaviour
{
    Rigidbody rbBomb; //BombNet ��ũ��Ʈ�� ������ ������Ʈ�� Rigidbody ������Ʈ
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        if (!IsOwner) return; //IsOwner�� false�� �� BombNet ������Ʈ�� �����ڰ� �ƴϹǷ� �ʱ�ȭ���� ����

        rbBomb = GetComponent<Rigidbody>();
       
        shotBombRpc(); //��ź �߻� ȿ���� ��� �� ��ź�� ���� ���ϴ� RPC ȣ��

        Invoke("killBombRpc", 4.0f); //4�� �ڿ� killBombRpc() ȣ���ؼ� ��ź�� �ı�
    }

    // Update is called once per frame
    void Update()
    {

    }

    [Rpc(SendTo.Owner)]
    void shotBombRpc()
    {
        SoundManager.Instance.f_PlaySFX(SoundName.SFX_CannonFire, 0.7f); //��ź �߻� ȿ���� ���
        
        rbBomb.AddForce(transform.up * GameManager.Instance.ForceBomb);
    }

    [Rpc(SendTo.Server)] //Despawn()�� server RPC�� ����
    void killBombRpc()
    {
        NetworkObject.Despawn(); //Despawn()�� Spawn()�� �ݴ븻: ������ de�� ���شٴ� ��
    }

    private void OnCollisionEnter(Collision collision)
    {
        //���� ��ġ�� BombRed(tag = Bomb)
        string tag = collision.gameObject.tag;  //�浹�� ���ü�� tag
        if (tag == "Plane")                     //���� �ε���
        {
            makeExplosionRpc(); //���� ȿ�� ����
            killBombRpc();      //��ź �ı�
        }
        else if (tag == "Player") //���� ��ũ�� �浹
        {
            PlayerHealthNet player = collision.gameObject.GetComponent<PlayerHealthNet>(); //�浹�� ��ũ�� PlayerHealthNet ������Ʈ ��������
            if (player != null)
            {
                player.decHealthRpc(); //�浹�� ��ũ�� ü�� ���� RPC ȣ��

                /* [��� �߰�]
                 * ���ۿ� �� ���� ������ �ڱ� Rigidbody�� Force ���ϵ��� Owner RPC ȣ��
                 * (��Ʈ��ũ ���� Ư�� ���� ��PlayerHealthNet�� �ڼ��� �ּ�)
                 */
                Vector3 direction = (collision.transform.position - transform.position).normalized;
                player.f_ApplyPushForceRpc(direction);
            }

            //���� ȿ�� �� ��ź �ı�
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
        prefab.GetComponent<NetworkObject>().Spawn(); //server RPC�� Spawn() �޼ҵ� ȣ�� ����

        SoundManager.Instance.f_PlaySFX(SoundName.SFX_Explosive, 0.7f); //���� ȿ���� ���
    }
}
